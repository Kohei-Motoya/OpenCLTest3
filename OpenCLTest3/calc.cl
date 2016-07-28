/*
 * calc.cl
 */

/*------------------------------------------------------------------------------------------------------------
 * 点（x, y）について、発散までの計算回数を返す
 */
int getCalcNum(const float x, const float y, const int max_calc)
{
    int   count = 0;
    float prevX = 0.0f;
    float prevY = 0.0f;
    
    for (int i = 0; i < max_calc + 1; i++){
        count = i + 1;
        float newX = (prevX * prevX) - (prevY * prevY) + x;
        float newY = (2.0f * prevX * prevY) + y;
        
        if ((newX * newX) + (newY * newY) > 4.0f){
            // 発散する
            break;
        }else{
            prevX = newX;
            prevY = newY;
        }
    }
    return count;
}

/*------------------------------------------------------------------------------------------------------------
 * カーネル関数
 * １ピクセル分の計算回数を求め、結果を格納する
 *
 * max_calc: 最大計算回数
 * start_x : 表示領域の左下のx座標
 * start_y : 表示領域の左下のy座標
 * step    : 表示領域における１ピクセルの幅
 * out     : 計算結果を格納する整数配列へのポインタ
 */
__kernel void calcMandelbrot(const int max_calc,
                             const float start_x,
                             const float start_y,
                             const float step,
                             __global int *out)
{
    size_t x = get_global_id(0);
    size_t y = get_global_id(1);
    size_t width = get_global_size(0);
    
    int index = (width * y) + x;
    
    float fx = mad(step, (float)x, start_x); // a * b + c
    float fy = mad(step, (float)y, start_y);
    
    out[index] = getCalcNum(fx, fy, max_calc);
}
