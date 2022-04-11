#include "path.hpp"

bool FindPath(Card **board, int boardHeight, int boardWidth, Pos *init, Pos *path) {
    return test();
}

void DrawPath(Pos *path) {return;}

/*
Lưu ý:  - Windows bị bủg không dám chạy biên dịch nên nhờ Minh biên dịch giùm :>
        - Từ hàm CheckLineX là số 1 đếm xuống đến số 9 sẽ là hàm chính check đường đi
        - Trục Y là hàng, trục X là cột

        Một vài biến khai báo đã sử dụng:
        - p1, p2 là hai biến lưu hai card chọn thứ nhất và card chọn thứ hai
        - pMinX lưu toạ độ của điểm có X nhỏ nhất (pMaxX: X lớn nhất) giữa 2 điểm đã chọn
        - pMinY lưu toạ độ của điểm có Y nhỏ nhất (pMaxY: Y lớn nhất) giữa 2 điểm đã chọn
    
*/
bool CheckLineX(int x1, int x2, char **board, int y)
{
    int max = x1, min = x2;
    if(x1 < x2)
    {
        max = x2;
        min = x1;
    }

    for(int x = min; x <= max; x++)
        if(board[y][x].status() == "none")
            return false;
    
    return true;
}

bool CheckLineY(int y1, int y2, char**board, int x)
{
    //Tìm max giữa y1 và y2
    int max = y1, min = y2;
    if(y1 < y2)
    {
        max = y2;
        min = y1;
    }

    //Chạy đường thẳng y để kiểm tra đường đi
    for(int y = min; y <= max; y++)
        if(board[y][x].status() == "none")
            return false;

    return true;
    
}

bool CheckI(Pos p1, Pos p2, char **board)
{
    //Kiểm tra nếu như hai điểm có cùng toạ độ X
    if(p1.x == p2.x)
        if(!CheckLineY(p1.y, p2.y, board, p1.x))
            return false;

    
    else
    {
        //Kiểm tra nếu như hai điểm có cùng toạ độ Y
        if(p1.y == p2.y)
        {
            if(!CheckLineX(p1.x, p2.x, board, p1.y));
                return false;
        }

        //Nếu như hai điểm không có cùng toạ độ x, y
        else
            return false;
    }

    return true;
}

//===============================Check Z=============================================

bool CheckZVertical(Pos pMinY, Pos pMaxY, char** board) 
{        
    for (int y = pMinY.y + 1; y < pMaxY.y; y++) 
    {   
        if (CheckLineY(pMinY.y, y, board, pMinY.x)         
         && CheckLineX(pMinY.x, pMaxY.x, board, y)  
         && CheckLineY(y, pMaxY.y, board, pMaxY.x)) 
            return true;
    }
    return false;
}

bool CheckZHorizontal(Pos pMinX, Pos pMaxX, char** board)
{

    for (int x = pMinX.x + 1; x < pMaxX.x; x++)
    {
        if (CheckLineX(pMinX.x, x, board, pMinX.y)
         && CheckLineY(pMinX.y, pMaxX.y, board, x)
         && CheckLineX(x, pMaxX.x, board, pMaxX.y))
            return true;
    }

    return false;
}

bool CheckZ(Pos p1, Pos p2, char** board)
{
    Pos pMinY = p1, pMaxY = p2;
    if(p1.y > p2.y)
    {
        pMinY = p2;
        pMaxY = p1;
    }

    //Check đường đi chữ Z nằm dọc (lật chữ Z sang phải/trái một góc 90 dộ đều được) và check luôn được chữ L
    if(CheckZVertical(pMinY, pMaxY, board))
        return true;

    else
    {
        Pos pMinX = p1, pMaxX = p2;
        if(p1.x > p2.x)
        {
            pMinX = p2;
            pMaxX = p1;
        }

        //Check đường đi chữ Z và check luôn được chữ L
        if(CheckZHorizontal(pMinX, pMaxX, board))
            return true;
    }

    return false;
}
//===================================================================================


//===================================Check U==============================================

//Check chữ U bình thường
bool CheckU_Up(Pos pMinX, Pos pMaxX, char** board, int height)
{
    for(int y = pMinX.y + 1; y < height; y++)
    {
        if(CheckLineY(pMinX.y, y, board, pMinX.x)
        && CheckLineX(pMinX.x, pMaxX.x, board, y)
        && CheckLineY(pMaxX.y, y, board, pMaxX.x))
            return true;

        //Nếu đường đi Y đi tới được viền của bảng
        else if (y == height - 1)
        {   
            //Kiểm tra nếu cả hai điểm p1 và p2 có đường đi vẽ chạm tới viền ngoài của bảng => vẽ được chữ U
            if(CheckLineY(pMinX.y, y, board, pMinX.x) && CheckLineY(pMaxX.y, y, board, pMaxX.x))
                return true;
        }
    }
    return false;
}

//Check chữ U lật xuống (giống kí hiệu "và" trong toán tập hợp)
bool CheckU_Down(Pos pMinX, Pos pMaxX, char** board, int height)
{
    for(int y = pMinX.y - 1; y >= 0; y--)
    {
        if(CheckLineY(pMinX.y, y, board, pMinX.x)
        && CheckLineX(pMinX.x, pMaxX.x, board, y)
        && CheckLineY(pMaxX.y, y, board, pMaxX.x))
            return true;

        //Nếu đường đi Y đi tới được viền của bảng
        else if (y == 0)
        {   
            //Kiểm tra nếu cả hai điểm p1 và p2 đều có đường đi vẽ chạm tới viền ngoài của bảng => vẽ được chữ U
            if(CheckLineY(pMinX.y, y, board, pMinX.x) && CheckLineY(pMaxX.y, y, board, pMaxX.x))
                return true;
        }
    }
    return false;
}

//Check chữ U lật 90 độ sang trái
bool CheckU_Left(Pos p1, Pos p2, char** board, int width)
{
    Pos pMinY = p1, pMaxY = p2;
    if(p1.y > p2.y)
    {
        pMinY = p1;
        pMaxY = p2;
    }

    for(int x = pMinY.x + 1; x < width; x++)
    {
        if(CheckLineX(pMinY.x, x, board, pMinY.y)
        && CheckLineY(pMinY.y, pMaxY.y, board, x)
        && CheckLineX(pMaxY.x, x, board, pMaxY.y))
            return true;

        //Đường đi X chạm tới viền bảng
        else if(x == width - 1)
        {
        //Kiểm tra hai đường X của hai điểm p1 và p2 có chạm ra ngoài viền bảng không (nếu được nghĩa là đi chữ U bên ngoài)
            if((CheckLineX(pMinY.x, x, board, pMinY.y) && (CheckLineX(pMaxY.x, x, board, pMaxY.y))
                return true;
        }
    }
    
    return false;
}

//Check chữ U lật 90 độ sang phải
bool CheckU_Right(Pos p1, Pos p2, char** board)
{
    Pos pMinY = p1, pMaxY = p2;
    if(p1.y > p2.y)
    {
        pMinY = p1;
        pMaxY = p2;
    }

    for(int x = pMinY.x - 1; x >= 0; x--)
    {
        if(CheckLineX(pMinY.x, x, board, pMinY.y)
        && CheckLineY(pMinY.y, pMaxY.y, board, x)
        && CheckLineX(pMaxY.x, x, board, pMaxY.y))
            return true;

        else if(x == 0)
        {
            if((CheckLineX(pMinY.x, x, board, pMinY.y) && (CheckLineX(pMaxY.x, x, board, pMaxY.y))
                return true;
        }
    }
    
    return false;
}

//Hai hàm check chữ U
bool CheckU(Pos p1, Pos p2, char** board, int height, int width)
{   
    //Nếu hai điểm cùng năm trên viền ngoài chiều rộng
    if((p1.y == height - 1 || p1.y == 0) && (p1.y == p2.y))
        return true;

    //Nếu hai điểm cùng nằm trên viền ngoài chiều dài 
    else if((p1.x == width - 1 || p1.x == 0) && (p1.x == p2.x))
        return true;

    //Trường hợp còn lại
    else
    {
        //Check U up & down
        Pos pMinX = p1, pMaxX = p2;
        if(p1.x > p2.x)
        {
            pMinX = p2;
            pMaxX = p1;
        }
        
        if(CheckU_Up(pMinX, pMaxX, board, height))
            return true;
        else if(CheckU_Down(pMinX, pMaxX, board, height))
            return true;

        //Check U left & right
        else
        {
            Pos pMinY = p1, pMaxY = p2;
            if(p1.y > p2.y)
            {
                pMinY = p2;
                pMaxY = p1;
            }

            if(CheckU_Left(pMinY, pMaxY, board, width))
                return true;

            else if(CheckU_Rigth(pMinY, pMaxY, board))
                return true;
        }
    }

    return false;
}
//=======================================================================================

bool CheckPaths(Pos p1, Pos p2, char **board, int height, int width)
{
    //Kiểm tra ngay hai giá trị có giống nhau hay không
    if(board[p1.y][p1.x] != board[p2.y][p2.x])
        return false;
    
    //Kiểm tra các đường đi
    else if(CheckI(p1, p2, board))
            return true;

    else if(CheckZ(p1, p2, board))
            return true;
    
    else if(CheckU(p1, p2, board, height, width))
            return true;

    //Không đi được đường nào cả
    else 
            return false;

}