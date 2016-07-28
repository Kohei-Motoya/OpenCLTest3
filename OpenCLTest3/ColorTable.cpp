//
//  ColorTable.cpp
//  OpenCLTest3
//

#include <stdio.h>

#ifdef __APPLE__
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include "ColorTable.h"

/*------------------------------------------------------------------------------------------------------------
 * コンストラクタ
 */
ColorTable::ColorTable(const int numColorIndex)
{
    if (numColorIndex <= 0) {
        fprintf(stderr, "%s Error: numColorIndex must be > 0\n", __FUNCTION__);
        return;
    }
    
    mNumColorIndex = numColorIndex;
    mValueR = new float[mNumColorIndex];
    mValueG = new float[mNumColorIndex];
    mValueB = new float[mNumColorIndex];
    fillColorTable();
}

/*------------------------------------------------------------------------------------------------------------
 * デストラクタ
 */
ColorTable::~ColorTable()
{
    if (mValueB != 0) {
        delete [] mValueB;
        mValueB = 0;
    }
    if (mValueG != 0) {
        delete [] mValueG;
        mValueG = 0;
    }
    if (mValueR != 0) {
        delete [] mValueR;
        mValueR = 0;
    }
}

/*------------------------------------------------------------------------------------------------------------
 * indexに応じた色値（R成分）を返す
 */
float ColorTable::getValueR(const int index) const
{
    if (index < 0) {
        fprintf(stderr, "%s Error: index must be > 0\n", __FUNCTION__);
        return 0.0f;
    }
    
    return mValueR[index % mNumColorIndex];
}

/*------------------------------------------------------------------------------------------------------------
 * indexに応じた色値（G成分）を返す
 */
float ColorTable::getValueG(const int index) const
{
    if (index < 0) {
        fprintf(stderr, "%s Error: index must be > 0\n", __FUNCTION__);
        return 0.0f;
    }
    
    return mValueG[index % mNumColorIndex];
}

/*------------------------------------------------------------------------------------------------------------
 * indexに応じた色値（B成分）を返す
 */
float ColorTable::getValueB(const int index) const
{
    if (index < 0) {
        fprintf(stderr, "%s Error: index must be > 0\n", __FUNCTION__);
        return 0.0f;
    }
    
    return mValueB[index % mNumColorIndex];
}

/*------------------------------------------------------------------------------------------------------------
 * 色値のテーブルに値をセットする
 */
void ColorTable::fillColorTable()
{
    const float step = 100.0f;
    
    for (int i = 0; i < mNumColorIndex; i++) {
        mValueR[i] = float((sin(((M_PI * 2.0 * (float)i) / step + M_2_PI)) + 1.0f) / 2.0f);
        mValueG[i] = float((sin(((M_PI * 3.0 * (float)i) / step + M_2_PI)) + 1.0f) / 2.0f);
        mValueB[i] = float((sin(((M_PI * 6.0 * (float)i) / step + M_2_PI)) + 1.0f) / 2.0f);
    }
}
