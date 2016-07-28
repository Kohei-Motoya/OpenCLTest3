//
//  VdcMapper.cpp
//  OpenCLTest3
//

#include "VdcMapper.h"


#include "MyError.h"
#include "VdcMapper.h"

const float DEFAULT_MAXY = 1.0f;

/*------------------------------------------------------------------------------------------------------------
 * コンストラクタ
 */
VdcMapper::VdcMapper(const unsigned int width,
                     const unsigned int height)
    :mDcWidth(width), mDcHeight(height),
    mVcMaxY(DEFAULT_MAXY),
    mVcX0(0.0f), mVcY0(0.0f)
{
    if (mDcWidth <= 0 || mDcHeight <= 0) {
        fprintf(stderr, "%s: width/height must be > 0\n", __FUNCTION__);
        throw;
    }
    
    setMaxY(DEFAULT_MAXY);
    initCenter(DEFAULT_MAXY);
}

/*------------------------------------------------------------------------------------------------------------
 * デストラクタ
 */
VdcMapper::~VdcMapper()
{
}

/*------------------------------------------------------------------------------------------------------------
 * 表示領域の高さ（半分の値）をセットする
 */
void VdcMapper::setMaxY(const double maxY)
{
    double diff = maxY - mVcMaxY;
    
    mVcMaxY = maxY;
    mVcY0 -= diff;
    mVcX0 -= ((diff*(double)mDcWidth) / (double)mDcHeight);
}

/*------------------------------------------------------------------------------------------------------------
 * 表示領域中心のx座標をセットする
 */
void VdcMapper::setCenterVcX(const float vcX)
{
    mVcX0 = vcX - (getVcWidth() / 2.0f);
}

/*------------------------------------------------------------------------------------------------------------
 * 表示領域中心のy座標をセットする
 */
void VdcMapper::setCenterVcY(const float vcY)
{
    mVcY0 = vcY - (getVcHeight() / 2.0f);
}

/*------------------------------------------------------------------------------------------------------------
 * 現在の表示領域中心のx座標を返す
 */
double VdcMapper::getCenterVcX() const
{
    return (mVcX0 + (getVcWidth() / 2.0f));
}

/*------------------------------------------------------------------------------------------------------------
 * 現在の表示領域中心のy座標を返す
 */
double VdcMapper::getCenterVcY() const
{
    return (mVcY0 + (getVcHeight() / 2.0f));
}

/*------------------------------------------------------------------------------------------------------------
 * ピクセル（dcX, dcY）を表示領域の中心ピクセルにする
 */
void VdcMapper::setCenterWithDcXY(const int dcX, const int dcY)
{
    mVcX0 = mVcX0 + getVcX(dcX) - getVcX((unsigned int)(0.5f * (double)mDcWidth + 0.5f));
    mVcY0 = mVcY0 + getVcY(dcY) - getVcY((unsigned int)(0.5f * (double)mDcHeight + 0.5f));
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
void VdcMapper::initCenter(const double maxY)
{
    mVcY0 = -1.0f * maxY;
    mVcX0 = ((double)mDcWidth * mVcY0) / (double)mDcHeight;
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
double VdcMapper::getVcWidth() const
{
    return (getVcHeight() * (double)mDcWidth) / (double)mDcHeight;
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
double VdcMapper::getVcHeight() const
{
    return (mVcMaxY * 2.0f);
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
double VdcMapper::getStep() const
{
    return (getVcHeight() / (double)mDcHeight);
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
double VdcMapper::getVcX(const int dcX) const
{
    double vdcX;
    vdcX = ((getVcWidth() * (double)dcX) / (double)mDcWidth) + mVcX0;
    return vdcX;
}

/*------------------------------------------------------------------------------------------------------------
 *
 */
double VdcMapper::getVcY(const int dcY) const
{
    double vdcY;
    vdcY = ((getVcHeight() * (double)dcY) / (double)mDcHeight) + mVcY0;
    return vdcY;
}