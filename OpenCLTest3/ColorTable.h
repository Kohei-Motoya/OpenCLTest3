//
//  ColorTable.hpp
//  OpenCLTest3
//

#ifndef __COLOR_TABLE_H__
#define __COLOR_TABLE_H__

#define DEFAULT_NUM_COLORS 1000

/*------------------------------------------------------------------------------------------------------------
 *
 */
class ColorTable {
public:
    ColorTable(const int numColorIndex = DEFAULT_NUM_COLORS);
    virtual ~ColorTable();
    
    float getValueR(const int index) const;
    float getValueG(const int index) const;
    float getValueB(const int index) const;
    
private:
    void fillColorTable();
    
private:
    int mNumColorIndex;  // 色値テーブルの大きさ
    float* mValueR;      // R用ピクセル値のテーブル
    float* mValueG;      // G用ピクセル値のテーブル
    float* mValueB;      // B用ピクセル値のテーブル
    
};

#endif /* __COLOR_TABLE_H__ */
