//
//  VdcMapper.hpp
//  OpenCLTest3
//

#ifndef __VDC_MAPPER_H__
#define __VDC_MAPPER_H__

const int DEFAULT_DCWIDTH  = 600;
const int DEFAULT_DCHEIGHT = 800;

/*------------------------------------------------------------------------------------------------------------
 * VC（仮想座標系（xy平面を表し、実装値をとる））と、
 * DC（装置座標系（表示領域を表し、整数値をとる））の対応を管理する
 */

class VdcMapper {
public:
    VdcMapper(const unsigned int width = DEFAULT_DCWIDTH,
              const unsigned int height = DEFAULT_DCHEIGHT);
    virtual ~VdcMapper();
    
    // setter
    void setMaxY(const double maxY);
    void setCenterWithDcXY(const int dcX, const int dcY);
    void setCenterVcX(const float vcX);
    void setCenterVcY(const float vcY);
    
    // getter
    unsigned int getDcWidth() const {return mDcWidth;}
    unsigned int getDcHeight() const {return mDcHeight;}
    double getVcX0() const {return mVcX0;}
    double getVcY0() const {return mVcY0;}
    double getMaxY() const {return mVcMaxY;}
    double getStep() const;
    
    double getCenterVcX() const;
    double getCenterVcY() const;
    
private:
    void initCenter(const double maxY);
    double getVcWidth() const;
    double getVcHeight() const;
    double getVcX(const int dcX) const;
    double getVcY(const int dcY) const;
    
private:
    unsigned int mDcWidth;   // 表示領域の幅（DC）
    unsigned int mDcHeight;  // 表示領域の高さ（DC）
    double mVcX0;            // 表示領域左下のx(VC)座標
    double mVcY0;            // 表示領域左下のy(VC)座標
    double mVcMaxY;          // 表示領域の高さの半分(VC)
};

#endif /* __VDC_MAPPER_H__ */
