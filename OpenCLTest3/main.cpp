//
//  main.cpp
//  OpenCLTest3
//

#include <iostream>
#include <stdio.h>

#include "MyError.h"
#include "ColorTable.h"
#include "Bitmap.h"
#include "Mandelbrot.h"

#define IMAGE_SAVE_LOCATION "/Somewhere/output.bmp"

const int CanvasWidth   = 800;   // ビットマップ画面の幅
const int CanvasHeight  = 600;   // ビットマップ画面の高さ
const int NumColorIndex = 1000;  // 最大色数
const int MaxCalcNum    = 1000;  // 最大計算回数

/*------------------------------------------------------------------------------------------------------------
 * マンデルブロ集合を描画したビットマップファイルを生成するプログラム
 */

int main(int argc, const char * argv[])
{
    Mandelbrot *mb;
    double x0, y0, maxY;
    
    if (argc >= 4) {
        // 引数が指定された
        double height;
        int num_error = 0;
        
        if (sscanf(argv[1], "%lf", &x0) != 1) {
            fprintf(stderr, "Error: unexpected argument #1 (%s)\n", argv[1]);
            num_error++;
        }
        
        if (sscanf(argv[2], "%lf", &y0) != 1) {
            fprintf(stderr, "Error: unexpected argument #2 (%s)\n", argv[2]);
            num_error++;
        }
        
        if (sscanf(argv[3], "%lf", &height) != 1 || height < 0) {
            fprintf(stderr, "Error: unexpected argument #3 (%s)\n", argv[3]);
            num_error++;
        }
        
        if (num_error > 0) {
            fprintf(stderr, "\n");
            fprintf(stderr, "Usage: %s [center_x center_y half_height]\n", argv[0]);
            fprintf(stderr, "center_x: center x of the view area. (double)\n");
            fprintf(stderr, "center_y: center y of the view area. (double)\n");
            fprintf(stderr, "height  : height of the view area.   (double)\n");
            return 1;
        }
        maxY = height * 0.5f;
    }else{
        // 引数が足りないので、デフォルトの値を使用する
        x0   = 0.0f;
        y0   = 0.0f;
        maxY = 1.2f;
    }
    
    // マンデルブル集合を計算する
    try {
        mb = new Mandelbrot(CanvasWidth, CanvasHeight);
        mb->setMaxCalcNum(MaxCalcNum);   // 最大計算回数をセット
        mb->setCenterVcX(x0);            // 中心のx座標をセット
        mb->setCenterVcY(y0);            // 中心のy座標をセット
        mb->setMaxY(maxY);               // 表示領域の高さ（半分の値）をセット
        // mb->calculate();              // OepnCLを使わない実装
        mb->calculateWithOpenCL();       // OepnCLを使った実装
    } catch (MyError err) {
        fprintf(stderr, "Error: %s\n", err.cstr());
        return 1;
    }
    
    int numData = CanvasWidth * CanvasWidth; //画面のピクセル数
    int* calcnum = mb->data(); // 計算結果を格納した配列へのポインタを取得
    
    // R, G, Bのピクセル値（0から1.0fで正規化されている）を格納する配列を用意
    float* dataG = new float[numData];
    float* dataR = new float[numData];
    float* dataB = new float[numData];
    
    try {
        // 計算結果に応じたピクセル値をdataG, datar, dataBにセットする
        ColorTable ct = ColorTable(NumColorIndex);
        for (int y = 0; y < CanvasHeight; y++) {
            for (int x = 0; x < CanvasWidth; x++) {
                int index = (y * CanvasWidth) + x;
                int num = calcnum[index];
                if (num > MaxCalcNum) {
                    // マンデルブロ集合に属する点を黒色にする
                    dataG[index] = 0.0f;
                    dataR[index] = 0.0f;
                    dataB[index] = 0.0f;
                }else{
                    // 計算回数（num）に応じた色値にセットする
                    dataG[index] = ct.getValueG(num);
                    dataR[index] = ct.getValueR(num);
                    dataB[index] = ct.getValueB(num);
                }
            }
        }
    } catch (MyError err) {
        fprintf(stderr, "Error: %s\n", err.cstr());
        return 2;
    }
    
    try {
        // 計算結果をビットマップファイルに書き出す
        Bitmap bm;
        bm.create(CanvasWidth, CanvasHeight, dataR, dataG, dataB);
        bm.writeFile(IMAGE_SAVE_LOCATION);
        
        delete [] dataB;
        delete [] dataR;
        delete [] dataG;
        delete mb;
    } catch (MyError err) {
        fprintf(stderr, "Error: %s\n", err.cstr());
        return 3;
    }
    
    return 0;
}
