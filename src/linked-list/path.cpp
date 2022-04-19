#include "path.hpp"

using namespace std;

/*
Lưu ý:  - Windows bị bủg không dám chạy biên dịch nên nhờ Minh biên dịch giùm :>
        - Từ hàm CheckLineX là số 1 đếm xuống đến số 9 sẽ là hàm chính check
đường đi
        - Trục Y là hàng, trục X là cột

        Một vài biến khai báo đã sử dụng:
        - p1, p2 là hai biến lưu hai card chọn thứ nhất và card chọn thứ hai
        - pMinX lưu toạ độ của điểm có X nhỏ nhất (pMaxX: X lớn nhất) giữa 2
điểm đã chọn
        - pMinY lưu toạ độ của điểm có Y nhỏ nhất (pMaxY: Y lớn nhất) giữa 2
điểm đã chọn

*/
bool CheckLineX(int x1, int x2, List *board, int y) {
    int max = x1, min = x2;
    if (x1 < x2) {
        max = x2;
        min = x1;
    }

    for (int x = min; x <= max; x++)
        if (GetNode(board, {y, x})->data.status == STATUS_NONE) return false;

    return true;
}

bool CheckLineY(int y1, int y2, List *board, int x) {
    // Tìm max giữa y1 và y2
    int max = y1, min = y2;
    if (y1 < y2) {
        max = y2;
        min = y1;
    }

    // Chạy đường thẳng y để kiểm tra đường đi
    for (int y = min; y <= max; y++)
        if (GetNode(board, {y, x})->data.status == STATUS_NONE) return false;

    return true;
}

bool CheckI(Pos p1, Pos p2, List *board) {
    // Kiểm tra nếu như hai điểm có cùng toạ độ X
    if (p1.x == p2.x) {
        if (!CheckLineY(p1.y, p2.y, board, p1.x))
            return false;
    }
    else {
        // Kiểm tra nếu như hai điểm có cùng toạ độ Y
        if (p1.y == p2.y) {
            if (!CheckLineX(p1.x, p2.x, board, p1.y))
                return false;
        }

        // Nếu như hai điểm không có cùng toạ độ x, y
        else 
            return false;
    }

    return true;
}

//===============================Check
//Z=============================================

bool CheckZVertical(Pos pMinY, Pos pMaxY, List *board, Path &newPath) {
    for (int y = pMinY.y + 1; y <= pMaxY.y; y++) {
        if (CheckLineY(pMinY.y, y, board, pMinY.x) &&
            CheckLineX(pMinY.x, pMaxY.x, board, y) &&
            CheckLineY(y, pMaxY.y, board, pMaxY.x)) {
                
                //Điểm 1
                Append(newPath, pMinY);

                //Điểm 2
                Pos p2;
                p2.x = pMinY.x;
                p2.y = y;
                Append(newPath, p2);

                //Điểm 3
                Pos p3;
                p3.x = pMaxY.x;
                p3.y = y;
                Append(newPath, p3);

                //Điểm 4
                Append(newPath, pMaxY);

                return true;
            }
    }
    return false;
}

bool CheckZHorizontal(Pos pMinX, Pos pMaxX, List *board, Path &newPath) {
    for (int x = pMinX.x + 1; x <= pMaxX.x; x++) {
        if (CheckLineX(pMinX.x, x, board, pMinX.y) &&
            CheckLineY(pMinX.y, pMaxX.y, board, x) &&
            CheckLineX(x, pMaxX.x, board, pMaxX.y)) {

                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ Z I
                Pos p2;
                p2.y = pMinX.y;
                p2.x = x;
                Append(newPath, p2);

                //Góc chữ Z II
                Pos p3;
                p3.y = pMaxX.y;
                p3.x = x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);

                return true;
            }
    }

    return false;
}

bool CheckZ(Pos p1, Pos p2, List *board, Path &newPath) {
    Pos pMinY = p1, pMaxY = p2;
    if (p1.y > p2.y) {
        pMinY = p2;
        pMaxY = p1;
    }

    // Check đường đi chữ Z nằm dọc (lật chữ Z sang phải/trái một góc 90 dộ đều được) và check luôn được chữ L
    if (CheckZVertical(pMinY, pMaxY, board, newPath))
        return true;

    else {
        Pos pMinX = p1, pMaxX = p2;
        if (p1.x > p2.x) {
            pMinX = p2;
            pMaxX = p1;
        }

        // Check đường đi chữ Z và check luôn được chữ L
        if (CheckZHorizontal(pMinX, pMaxX, board, newPath)) return true;
    }

    return false;
}
//===================================================================================

//===================================Check U==============================================

// Check chữ U bình thường
bool CheckU_Up(Pos pMinX, Pos pMaxX, List *board, int height, Path &newPath) {
    //Nếu một trong hai điểm nằm viền bảng
    if(pMinX.y == height - 1) {
        if(CheckLineY(pMaxX.y, pMinX.y, board, pMaxX.x)) {

            //Điểm 1
            Append(newPath, pMinX);

            //Điểm 2
            Pos p2;
            p2.y = height;
            p2.x = pMinX.x;
            Append(newPath, p2);            

            //Điểm 3
            Pos p3;
            p3.y = height;
            p3.x = pMaxX.x;
            Append(newPath, p3);

            //Điểm 4
            Append(newPath, pMaxX);

            return true;
        }        
    }
    
    else if(pMaxX.y == height - 1) {
        if(CheckLineY(pMinX.y, pMaxX.y, board, pMinX.x)) {
            
            //Điểm 1
            Append(newPath, pMinX);

            //Điểm 2
            Pos p2;
            p2.y = height;
            p2.x = pMinX.x;
            Append(newPath, p2);            

            //Điểm 3
            Pos p3;
            p3.y = height;
            p3.x = pMaxX.x;
            Append(newPath, p3);

            //Điểm 4
            Append(newPath, pMaxX);

            return true;
        }
    }

    // Hai điểm chắc chắn nằm bên trong bảng
    else
    for (int y = pMinX.y + 1; y < height; y++) {
        if (CheckLineY(pMinX.y, y, board, pMinX.x) &&
            CheckLineX(pMinX.x, pMaxX.x, board, y) &&
            CheckLineY(pMaxX.y, y, board, pMaxX.x)) {

                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ U nằm ngoài bảng I
                Pos p2;
                p2.y = y;
                p2.x = pMinX.x;
                Append(newPath, p2);

                //Góc chữ U nằm ngoài bảng II
                Pos p3;
                p3.y = y;
                p3.x = pMaxX.x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);

                return true;
            }
            

        // Nếu đường đi Y đi tới được viền của bảng
        else if (y == height - 1) {
            // Kiểm tra nếu cả hai điểm p1 và p2 có đường đi vẽ chạm tới viền
            // ngoài của bảng => vẽ được chữ U
            if (CheckLineY(pMinX.y, y, board, pMinX.x) &&
                CheckLineY(pMaxX.y, y, board, pMaxX.x)) {
                    
                    //Đầu mút
                    Append(newPath, pMinX);

                    //Góc chữ U nằm ngoài bảng I
                    Pos p2;
                    p2.y = height;
                    p2.x = pMinX.x;
                    Append(newPath, p2);

                    //Góc chữ U nằm ngoài bảng II
                    Pos p3;
                    p3.y = height;
                    p3.x = pMaxX.x;
                    Append(newPath, p3);

                    //Đầu mút
                    Append(newPath, pMaxX);

                    return true;
                }
        }
    }
    return false;
}

// Check chữ U lật xuống (giống kí hiệu "và" trong toán tập hợp)
bool CheckU_Down(Pos pMinX, Pos pMaxX, List *board, int height, Path &newPath) {
    if(pMinX.y == 0) {
        if(CheckLineY(pMaxX.y, pMinX.y, board, pMaxX.x)) {
                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ U  I
                Pos p2;
                p2.y = -1;
                p2.x = pMinX.x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = -1;
                p3.x = pMaxX.x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);
                
                return true;
        }
    }

    else if(pMaxX.y == 0) {
        if(CheckLineY(pMinX.y, pMaxX.y, board, pMinX.x)) {
                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ U  I
                Pos p2;
                p2.y = -1;
                p2.x = pMinX.x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = -1;
                p3.x = pMaxX.x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);
                
                return true;
        }
    }

    else
    for (int y = pMinX.y - 1; y >= 0; y--) {
        if (CheckLineY(pMinX.y, y, board, pMinX.x) &&
            CheckLineX(pMinX.x, pMaxX.x, board, y) &&
            CheckLineY(pMaxX.y, y, board, pMaxX.x)) {

                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ U  I
                Pos p2;
                p2.y = y;
                p2.x = pMinX.x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = y;
                p3.x = pMaxX.x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);

                return true;
            }
            

        // Nếu đường đi Y đi tới được viền của bảng
        else if (y == 0) {
            // Kiểm tra nếu cả hai điểm p1 và p2 đều có đường đi vẽ chạm tới
            // viền ngoài của bảng => vẽ được chữ U
            if (CheckLineY(pMinX.y, y, board, pMinX.x) &&
                CheckLineY(pMaxX.y, y, board, pMaxX.x)) {

                //Đầu mút
                Append(newPath, pMinX);

                //Góc chữ U  I
                Pos p2;
                p2.y = -1;
                p2.x = pMinX.x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = -1;
                p3.x = pMaxX.x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxX);
 

                return true;
            }
                
        }
    }
    return false;
}

// Check chữ U lật 90 độ sang trái
bool CheckU_Left(Pos pMinY, Pos pMaxY, List *board, int width, Path &newPath) {
    if(pMinY.x == width - 1) {
        if(CheckLineX(pMaxY.x, pMinY.x, board, pMaxY.y)) {

            //Đầu mút
            Append(newPath, pMinY);

            //Góc chữ U  I
            Pos p2;
            p2.y = pMinY.y;
            p2.x = width;
            Append(newPath, p2);

            //Góc chữ U  II
            Pos p3;
            p3.y = pMaxY.y;
            p3.x = width;
            Append(newPath, p3);

            //Đầu mút
            Append(newPath, pMaxY);

            return true;
        }
    }

    else if(pMaxY.x == width - 1) {
        if(CheckLineX(pMinY.x, pMaxY.x, board, pMinY.y)) {

            //Đầu mút
            Append(newPath, pMinY);

            //Góc chữ U  I
            Pos p2;
            p2.y = pMinY.y;
            p2.x = width;
            Append(newPath, p2);

            //Góc chữ U  II
            Pos p3;
            p3.y = pMaxY.y;
            p3.x = width;
            Append(newPath, p3);

            //Đầu mút
            Append(newPath, pMaxY);

            return true;
        }
    }

    else
    for (int x = pMinY.x + 1; x < width; x++) {
        if (CheckLineX(pMinY.x, x, board, pMinY.y) &&
            CheckLineY(pMinY.y, pMaxY.y, board, x) &&
            CheckLineX(pMaxY.x, x, board, pMaxY.y)) {

                //Đầu mút
                Append(newPath, pMinY);

                //Góc chữ U  I
                Pos p2;
                p2.y = pMinY.y;
                p2.x = x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = pMaxY.y;
                p3.x = x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxY);
                return true;
            }
            

        //Đường đi X chạm tới viền bảng
        else if (x == width - 1) {
            // Kiểm tra hai đường X của hai điểm p1 và p2 có chạm ra ngoài viền
            // bảng không (nếu được nghĩa là đi chữ U bên ngoài)
            if (CheckLineX(pMinY.x, x, board, pMinY.y) &&
                CheckLineX(pMaxY.x, x, board, pMaxY.y)) {
                    //Đầu mút
                    Append(newPath, pMinY);

                    //Góc chữ U  I
                    Pos p2;
                    p2.y = pMinY.y;
                    p2.x = width;
                    Append(newPath, p2);

                    //Góc chữ U  II
                    Pos p3;
                    p3.y = pMaxY.y;
                    p3.x = width;
                    Append(newPath, p3);

                    //Đầu mút
                    Append(newPath, pMaxY);

                    return true;
                }
        }
    }

    return false;
}

// Check chữ U lật 90 độ sang phải
bool CheckU_Right(Pos pMinY, Pos pMaxY, List *board, Path &newPath) {
    if(pMinY.x == 0) {
        if(CheckLineX(pMaxY.x, pMinY.x, board, pMaxY.y)) {
            //Đầu mút
            Append(newPath, pMinY);

            //Góc chữ U  I
            Pos p2;
            p2.y = pMinY.y;
            p2.x = -1;
            Append(newPath, p2);

            //Góc chữ U  II
            Pos p3;
            p3.y = pMaxY.y;
            p3.x = -1;
            Append(newPath, p3);

            //Đầu mút
            Append(newPath, pMaxY);            

            return true;
        }
    }

    else if(pMaxY.x == 0) {
        if(CheckLineX(pMinY.x, pMaxY.x, board, pMinY.y)) {
            //Đầu mút
            Append(newPath, pMinY);

            //Góc chữ U  I
            Pos p2;
            p2.y = pMinY.y;
            p2.x = -1;
            Append(newPath, p2);

            //Góc chữ U  II
            Pos p3;
            p3.y = pMaxY.y;
            p3.x = -1;
            Append(newPath, p3);

            //Đầu mút
            Append(newPath, pMaxY);            

            return true;
        }
    }

    else
    for (int x = pMinY.x - 1; x >= 0; x--) {
        if (CheckLineX(pMinY.x, x, board, pMinY.y) &&
            CheckLineY(pMinY.y, pMaxY.y, board, x) &&
            CheckLineX(pMaxY.x, x, board, pMaxY.y)) {
                //Đầu mút
                Append(newPath, pMinY);

                //Góc chữ U  I
                Pos p2;
                p2.y = pMinY.y;
                p2.x = x;
                Append(newPath, p2);

                //Góc chữ U  II
                Pos p3;
                p3.y = pMaxY.y;
                p3.x = x;
                Append(newPath, p3);

                //Đầu mút
                Append(newPath, pMaxY);

                return true;
            }
            

        else if (x == 0) {
            if (CheckLineX(pMinY.x, x, board, pMinY.y) &&
                CheckLineX(pMaxY.x, x, board, pMaxY.y)) {

                    //Đầu mút
                    Append(newPath, pMinY);

                    //Góc chữ U  I
                    Pos p2;
                    p2.y = pMinY.y;
                    p2.x = -1;
                    Append(newPath, p2);

                    //Góc chữ U  II
                    Pos p3;
                    p3.y = pMaxY.y;
                    p3.x = -1;
                    Append(newPath, p3);

                    //Đầu mút
                    Append(newPath, pMaxY);

                    return true;
                }
               
        }
    }

    return false;
}

// Hai hàm check chữ U
bool CheckU(Pos p1, Pos p2, List *board, int height, int width, Path &newPath) {
    // Nếu hai điểm cùng năm trên viền ngoài chiều dài
    if ((p1.y == height - 1 || p1.y == 0) && (p1.y == p2.y)) {
        if(p1.y == height - 1) {
            //Đầu mút
            Append(newPath, p1);

            //Góc chữ U  I
            Pos p3;
            p3.y = height;
            p3.x = p1.x;
            Append(newPath, p3);

            //Góc chữ U  II
            Pos p4;
            p4.y = height;
            p4.x = p2.x;
            Append(newPath, p4);

            //Đầu mút
            Append(newPath, p2);            

            return true;
        }

        else if(p1.y == 0) {
            Append(newPath, p1);

            //Góc chữ U  I
            Pos p3;
            p3.y = -1;
            p3.x = p1.x;
            Append(newPath, p3);

            //Góc chữ U  II
            Pos p4;
            p4.y = -1;
            p4.x = p2.x;
            Append(newPath, p4);

            //Đầu mút
            Append(newPath, p2);   

            return true;
        }
    }

    // Nếu hai điểm cùng nằm trên viền ngoài chiều rộng
    else if ((p1.x == width - 1 || p1.x == 0) && (p1.x == p2.x)) {

        if(p1.x == width - 1) {
            Append(newPath, p1);

            //Góc chữ U  I
            Pos p3;
            p3.y = p1.y;
            p3.x = width;
            Append(newPath, p3);

            //Góc chữ U  II
            Pos p4;
            p4.y = p2.y;
            p4.x = width;
            Append(newPath, p4);

            //Đầu mút
            Append(newPath, p2);  

            return true;
        }

        else if(p1.x == 0) {
            Append(newPath, p1);

            //Góc chữ U  I
            Pos p3;
            p3.y = p1.y;
            p3.x = -1;
            Append(newPath, p3);

            //Góc chữ U  II
            Pos p4;
            p4.y = p2.y;
            p4.x = -1;
            Append(newPath, p4);

            //Đầu mút
            Append(newPath, p2);  

            return true;
        }
    }


    // Trường hợp còn lại
    else {
        // Check U up & down
        Pos pMinX = p1, pMaxX = p2;
        if (p1.x > p2.x) {
            pMinX = p2;
            pMaxX = p1;
        }

        if (CheckU_Up(pMinX, pMaxX, board, height, newPath))
            return true;

        else if (CheckU_Down(pMinX, pMaxX, board, height, newPath))
            return true;

        // Check U left & right
        else {
            Pos pMinY = p1, pMaxY = p2;
            if (p1.y > p2.y) {
                pMinY = p2;
                pMaxY = p1;
            }

            if (CheckU_Left(pMinY, pMaxY, board, width, newPath))
                return true;

            else if (CheckU_Right(pMinY, pMaxY, board, newPath))
                return true;
        }
    }

    return false;
}
//=======================================================================================
bool FindHint(List *board, int height, int width, Path &newPath) {
    for(int i = 0; i < height * width - 1; i++) {
        Pos p1;
        p1.y = i / width;
        p1.x = i % width;
        
        if (GetNode(board, {p1.y, p1.x})->data.status == STATUS_REMOVED) continue;

        GetNode(board, {p1.y, p1.x})->data.status = STATUS_HIGHLIGHTED;

        for(int j = i + 1; j < height * width; j++) {
            Pos p2;
            p2.y = j / width;
            p2.x = j % width;

            if (GetNode(board, {p2.y, p2.x})->data.status == STATUS_REMOVED) continue; 

            GetNode(board, {p2.y, p2.x})->data.status = STATUS_HIGHLIGHTED;

            if(CheckPaths(p1, p2, board, height, width, newPath)) {
                GetNode(board, {p1.y, p1.x})->data.status = STATUS_NONE;
                GetNode(board, {p2.y, p2.x})->data.status = STATUS_NONE;
                return true;
            }

            GetNode(board, {p2.y, p2.x})->data.status = STATUS_NONE;
        }

        GetNode(board, {p1.y, p1.x})->data.status = STATUS_NONE;
    }
    return false;
}

bool CheckPaths(Pos p1, Pos p2, List *board, int height, int width, Path &newPath) {

    //Làm mới newPath cho newPath.head và newPath.tail trỏ về NULL
    EmptyPath(newPath);
    
    // Kiểm tra ngay hai giá trị có giống nhau hay không
    if (GetNode(board, {p1.y, p1.x})->data.val != GetNode(board, {p2.y, p2.x})->data.val) return false;

    // Kiểm tra các đường đi
    else if (CheckI(p1, p2, board))
    {
        Append(newPath, p1);
        Append(newPath, p2);

        return true;
    }
        
    else if (CheckZ(p1, p2, board, newPath))
        return true;
        
    else if (CheckU(p1, p2, board, height, width, newPath))
         return true;
       
    // Không đi được đường nào cả
    else
        return false;
}

void DrawCorner(Pos &point, int lastDr, int currDr) {
    attron(COLOR_PAIR(2));

    if ((lastDr == DR_UP && currDr == DR_RIGHT) || (lastDr == DR_LEFT && currDr == DR_DOWN)) {
        mvaddch(point.y, point.x, ACS_ULCORNER);
        mvaddch(point.y, point.x + 1, ACS_HLINE);
        mvaddch(point.y + 1, point.x, ACS_VLINE);

        attroff(COLOR_PAIR(2));
        return ;
    }
    if ((lastDr == DR_RIGHT && currDr == DR_DOWN) || (lastDr == DR_UP && currDr == DR_LEFT)) {
        mvaddch(point.y, point.x, ACS_URCORNER);
        mvaddch(point.y, point.x - 1, ACS_HLINE);
        mvaddch(point.y + 1, point.x, ACS_VLINE);

        attroff(COLOR_PAIR(2));
        return ;
    }
    if ((lastDr == DR_DOWN && currDr == DR_LEFT) || (lastDr == DR_RIGHT && currDr == DR_UP)) {
        mvaddch(point.y, point.x, ACS_LRCORNER);
        mvaddch(point.y, point.x - 1, ACS_HLINE);
        mvaddch(point.y - 1, point.x, ACS_VLINE);  

        attroff(COLOR_PAIR(2));
        return ;
    }
    // left + up & down + right;
    mvaddch(point.y, point.x, ACS_LLCORNER);
    mvaddch(point.y, point.x + 1, ACS_HLINE);
    mvaddch(point.y - 1, point.x, ACS_VLINE);

    attroff(COLOR_PAIR(2));
}

void DrawLine(Pos point1, Pos point2, int &direction) {
    attron(COLOR_PAIR(2));

    int startPos;

    if (point1.x == point2.x) {
        if (point1.y < point2.y) {
            startPos = point1.y + CARD_HEIGHT / 2;
            direction = DR_DOWN;
        } else {
            startPos = point2.y + CARD_HEIGHT / 2;
            direction = DR_UP;
        }
        mvvline(startPos, point1.x, 0, abs(point2.y - point1.y) + 1 - (CARD_HEIGHT / 2) * 2);

        attroff(COLOR_PAIR(2));
        return ;
    }
    
    // equal y
    if (point1.x < point2.x) {
        startPos = point1.x + CARD_WIDTH / 2;
        direction = DR_RIGHT;
    } else {
        startPos = point2.x + CARD_WIDTH / 2;
        direction = DR_LEFT;
    }
    mvhline(point1.y, startPos, 0, abs(point2.x - point1.x) + 1 - (CARD_WIDTH / 2) * 2);

    attroff(COLOR_PAIR(2));
}

void DrawPath(List *board, int boardHeight, int boardWidth, Path currPath) {
    Pos lastPoint, currPoint;
    int lastDr, currDr;
    int offsetSadCase = 0;

    // get first point of the path
    pointNode *currPNode = currPath.head;

    // so that count start from 0
    int count = -1;

    while (currPNode) {
        ++count;
        
        int y = currPNode->data.y;
        int x = currPNode->data.x;
        
        if (currPNode != currPath.head) lastPoint = currPoint;

        if (currPNode->data.y == -1) {
            currPoint.y = - 1 - CARD_SPACE / 2 - CARD_HEIGHT / 2;
            ++y;
        } else if (currPNode->data.y == boardHeight) {
            currPoint.y = CARD_HEIGHT + CARD_HEIGHT / 2;
            --y;
        } else {
            currPoint.y = CARD_HEIGHT / 2;
        }

        if (currPNode->data.x == -1) {
            currPoint.x = - 1 - CARD_SPACE - CARD_WIDTH / 2;
            ++x;
        } else if (currPNode->data.x == boardWidth) {
            currPoint.x = CARD_WIDTH + CARD_WIDTH / 2;
            --x;
        } else {
            currPoint.x = CARD_WIDTH / 2;
        }

        currPoint.y += getbegy(GetNode(board, {y, x})->data.win.cover);
        currPoint.x += getbegx(GetNode(board, {y, x})->data.win.cover);
    
        if (currPNode == currPath.head) continue;

        // value return from checkpath by Hoang give dupliacate values in L case
        if (lastPoint.x == currPoint.x && lastPoint.y == currPoint.y) {
            ++offsetSadCase;
            continue;
        }

        lastDr = currDr;
        DrawLine(lastPoint, currPoint, currDr);

        if (count > 1 + offsetSadCase) DrawCorner(lastPoint, lastDr, currDr);
        // crucial to refresh, without it, random bugs may appear
        refresh();
    }
}
//Đủ 888 dòng