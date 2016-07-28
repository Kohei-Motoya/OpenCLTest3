//
//  Mandelbrot.cpp
//  OpenCLTest3
//

#include "Mandelbrot.h"

#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include "ClHelper.h"

#include "MyError.h"
#include "VdcMapper.h"
#include "Mandelbrot.h"

#define KERNEL_LOCATION "/Somewhere/calc.cl"

/*------------------------------------------------------------------------------------------------------------
 * コンストラクタ
 */
Mandelbrot::Mandelbrot(const int width, const int height)
{
    if (width <= 0 || height <= 0) {
        throw MyError("width/height must be > 0", __FUNCTION__);
        return;
    }
    
    mWidth = width;
    mHeight = height;
    mCount = width * height;
    
    mVdcMapper = new VdcMapper(mWidth, mHeight);
    mVdcMapper->setMaxY(1.2f); // default
    
    mHelper = new ClHelper();
    mHelper->preloadProgram(KERNEL_LOCATION);
    
    mData = new int[mCount];
    mMaxCalcNum = DEFAULT_MAX_CALCNUM;
    initData();
}

/*------------------------------------------------------------------------------------------------------------
 * デストラクタ
 */
Mandelbrot::~Mandelbrot()
{
    if (mData != 0) {
        delete [] mData;
        mData = 0;
    }
    
    if (mVdcMapper != 0) {
        delete mVdcMapper;
        mVdcMapper = 0;
    }
}

/*------------------------------------------------------------------------------------------------------------
 * マンデルブロ集合を計算する（OpenCLを使わない実装）
 * 計算結果はmDataに格納される
 */
void Mandelbrot::calculate()
{
    double startX       = mVdcMapper->getVcX0();
    double startY       = mVdcMapper->getVcY0();
    double step         = mVdcMapper->getStep();
    unsigned int width  = mVdcMapper->getDcWidth();
    unsigned int height = mVdcMapper->getDcHeight();
    
    int index = 0;
    for (unsigned int y = 0; y < height; y++) {
        double fy = startY + (step * (double)y);
        for (unsigned int x = 0; x < width; x++) {
            double fx = startX + (step * (double)x);
            mData[index] = getClacNum(fx, fy, mMaxCalcNum);
            index++;
        }
    }
}

/*------------------------------------------------------------------------------------------------------------
 * マンデルブロ集合を計算する（OpenCLを使った実装）
 * 計算結果はmDataに格納される
 */
void Mandelbrot::calculateWithOpenCL()
{
    // すでに作成しているコンテキストを取得する
    cl_context context = mHelper->getContext();
    
    // すでに作成しているコマンドキューを取得する
    cl_command_queue queue = mHelper->getCommandQueue();
    
    // すでに作成しているプログラムオブジェクトを取得する
    cl_program program = mHelper->getProgram();
    if (program == (cl_program)0) {
        throw MyError("program bug, program is zero", __FUNCTION__);
    }
    
    cl_int status;
    
    // カーネルを作成する
    cl_kernel kernel;
    kernel = clCreateKernel(program, "calcMandelbrot", &status);
    if(kernel == (cl_kernel)0) {
        printf("clCreateKernel failed\n");
        ClHelper::printError(status);
        throw MyError("failed to  create kernel.", __FUNCTION__);
    }
    
    // 結果を格納するためのメモリオブジェクトを作成する
    cl_mem out = clCreateBuffer(context,
                                CL_MEM_WRITE_ONLY,
                                sizeof(cl_int) * mCount,
                                0,
                                0);
    // カーネルに渡すパラメータをセットする
    const cl_int maxCalc = maxCalcNum();
    
    float startX = (float)mVdcMapper->getVcX0();
    float startY = (float)mVdcMapper->getVcY0();
    float step   = (float)mVdcMapper->getStep();
    
    status =  clSetKernelArg(kernel, 0, sizeof(cl_int),   (void *)&maxCalc);
    status += clSetKernelArg(kernel, 1, sizeof(cl_float), (void *)&startX);
    status += clSetKernelArg(kernel, 2, sizeof(cl_float), (void *)&startY);
    status += clSetKernelArg(kernel, 3, sizeof(cl_float), (void *)&step);
    status += clSetKernelArg(kernel, 4, sizeof(out),      (void *)&out);
    if (status != 0) {
        printf("clSetKernelArg failed\n");
        throw MyError("failed to set kernel arguments.", __FUNCTION__);
    }
    
    // カーネルの実行をリクエストする
    cl_uint work_dim = 2; // x, y
    size_t  global_work_size[] = {(size_t)mWidth, (size_t)mHeight};
    
    
    status = clEnqueueNDRangeKernel(queue,
                                    kernel,
                                    work_dim,
                                    NULL,
                                    global_work_size,
                                    0,
                                    0,
                                    NULL,
                                    NULL);
    if (status != 0) {
        ClHelper::printError(status);
        throw MyError("clEnqueueNDRangeKernel failed.", __FUNCTION__);
    }
    
    
    // 計算結果をデバイスのメモリからホストにコピーする
    status = clEnqueueReadBuffer(queue,
                                 out,
                                 CL_TRUE,
                                 0,
                                 sizeof(cl_int) * mCount,
                                 mData,
                                 0,
                                 NULL,
                                 NULL);
    if (status != 0) {
        ClHelper::printError(status);
        throw MyError("clEnqueueCopyBuffer failed.", __FUNCTION__);
    }
    
    // 作成したメモリオブジェクトを解放する
    clReleaseMemObject(out);
}

/*------------------------------------------------------------------------------------------------------------
 * 中心のピクセル（dcX, dcY）を指定する
 * （GUIなどで、マウスクリックされた点を画面の中心にセットする場合に使用する）
 */
void Mandelbrot::setCenterWithDcXY(const int dcX, const int dcY)
{
    mVdcMapper->setCenterWithDcXY(dcX, dcY);
}

/*------------------------------------------------------------------------------------------------------------
 * 表示に用いる中心のx座標（VC）を指定する
 */
void Mandelbrot::setCenterVcX(const double vcX)
{
    mVdcMapper->setCenterVcX(vcX);
}


/*------------------------------------------------------------------------------------------------------------
 * 表示に用いる中心のy座標（VC）を指定する
 */
void Mandelbrot::setCenterVcY(const double vcY)
{
    mVdcMapper->setCenterVcY(vcY);
}

/*------------------------------------------------------------------------------------------------------------
 * 現在セットされている中心のx座標（VC）を取得する
 */
double Mandelbrot::getCenterVcX() const
{
    return mVdcMapper->getCenterVcX();
}

/*------------------------------------------------------------------------------------------------------------
 * 現在セットされている中心のy座標（VC）を取得する
 */
double Mandelbrot::getCenterVcY() const
{
    return mVdcMapper->getCenterVcY();
}

/*------------------------------------------------------------------------------------------------------------
 * 表示領域の高さの半分の値（VC）をセットする
 */
void Mandelbrot::setMaxY(const double maxY)
{
    mVdcMapper->setMaxY(maxY);
}

/*------------------------------------------------------------------------------------------------------------
 * 現在セットされている表示領域の高さの半分の値（VC）を取得する
 */
double Mandelbrot::maxY() const
{
    return mVdcMapper->getMaxY();
}

/*------------------------------------------------------------------------------------------------------------
 * mDataを初期化する
 */
void Mandelbrot::initData()
{
    for (int i = 0; i < mCount; i++) {
        mData[i] = -1;
    }
}

/*------------------------------------------------------------------------------------------------------------
 * 与えられた（x, y）について計算回数を求める
 * （OpenCLを使わない実装で使用）
 */
int Mandelbrot::getClacNum(const double x,
                           const double y,
                           const int maxCalcNum) const
{
    int count = 0;
    double prevX = 0.0f;
    double prevY = 0.0f;
    
    for (int i = 0; i < maxCalcNum + 1; i++) {
        count = i + 1;
        double newX = (prevX * prevX) - (prevY * prevY) + x;
        double newY = 2.0f * prevX * prevY +y;
        
        if ((newX * newX) + (newY * newY) > 4.0f) {
            break;
        }else{
            prevX = newX;
            prevY = newY;
        }
    }
    
    return count;
}