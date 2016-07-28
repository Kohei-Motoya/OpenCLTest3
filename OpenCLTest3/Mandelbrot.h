//
//  Mandelbrot.hpp
//  OpenCLTest3
//

#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

class VdcMapper;
class ClHelper;

static const int DEFAULT_WIDTH  = 800;
static const int DEFAULT_HEIGHT = 600;
static const int DEFAULT_MAX_CALCNUM = 1000;


/*------------------------------------------------------------------------------------------------------------
 *
 */
class Mandelbrot {
public:
    Mandelbrot(const int width = DEFAULT_WIDTH,
               const int height = DEFAULT_HEIGHT);
    virtual ~Mandelbrot();
    
    void calculate();
    void calculateWithOpenCL();
    
    void setCenterWithDcXY(const int dcX, const int dcY);
    double getCenterVcX() const;
    double getCenterVcY() const;
    void setCenterVcX(const double vcX);
    void setCenterVcY(const double vcY);
    
    int* data() const {return mData;}
    int width() const {return mWidth;};
    int height() const {return mHeight;}
    int maxCalcNum() const {return mMaxCalcNum;}
    double maxY() const;
    
    void setMaxCalcNum(const int num) {mMaxCalcNum = num;}
    void setMaxY(const double maxY);
    
private:
    void initData();
    int getClacNum(const double x, const double y, const int maxCalcNum) const;
    
private:
    int mWidth;       // 表示画面の幅（ピクセル）
    int mHeight;      // 表示画面の高さ（ピクセル）
    int mCount;       // 画素の数（mWidth * mHeight）
    int mMaxCalcNum;  // 発散しないと判断するための最大計算回数
    int* mData;       // 計算回数を格納するバッファ
    
    VdcMapper *mVdcMapper;
    ClHelper *mHelper;
    
};

#endif /* __MANDELBROT_H__ */
