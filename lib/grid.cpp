#include "grid.h"
#include <vector>

Grid::Grid(int _width, int _height)
{
    width = _width;
    height = _height;
    midWidth = width/2;
    midHeight = height/2;
    tiles = vector(height, string(width, empty));
}   

const vector<string>& Grid::GetTiles() const { return tiles; }

void Grid::SetTile(int x, int y, char c)
{
    tiles[y][x] = c;
}

void Grid::SetTiles(int x, int y, const string&& str)
{
    str.copy(tiles[y].data(), str.size(), x);
}

void Grid::SetRow(int y, const string& str)
{
    tiles[y] = str;
}

void Grid::SetColumn(int x, const string& str)
{
    for (int y = 0; y < height; y++)
        tiles[y][x] = str[y];
}

void Grid::DrawRectangle(int x1, int y1, int x2, int y2, char c)
{
    for (int x = x1; x <= x2; x++)
        for (int y = y1; y <= y2; y++)
            grid.SetTile(x, y, c);
}

void Grid::DrawLine(int x1, int y1, int x2, int y2, char c)
{
    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
    dx=x2-x1;
    dy=y2-y1;
    dx1=abs(dx);
    dy1=abs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;
    if(dy1<=dx1)
    {
     if(dx>=0)
     {
      x=x1;
      y=y1;
      xe=x2;
     }
     else
     {
      x=x2;
      y=y2;
      xe=x1;
     }
     tiles[y][x] = c;
     for(i=0;x<xe;i++)
     {
      x=x+1;
      if(px<0)
      {
       px=px+2*dy1;
      }
      else
      {
       if((dx<0 && dy<0) || (dx>0 && dy>0))
       {
        y=y+1;
       }
       else
       {
        y=y-1;
       }
       px=px+2*(dy1-dx1);
      }
      tiles[y][x] = c;
     }
    }
    else
    {
     if(dy>=0)
     {
      x=x1;
      y=y1;
      ye=y2;
     }
     else
     {
      x=x2;
      y=y2;
      ye=y1;
     }
     tiles[y][x] = c;
     for(i=0;y<ye;i++)
     {
      y=y+1;
      if(py<=0)
      {
       py=py+2*dx1;
      }
      else
      {
       if((dx<0 && dy<0) || (dx>0 && dy>0))
       {
        x=x+1;
       }
       else
       {
        x=x-1;
       }
       py=py+2*(dx1-dy1);
      }
      tiles[y][x] = c;
     }
    }
}

void Grid::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, char c)
{
    DrawLine(x1, y1, x2, y2, c);
    DrawLine(x2, y2, x3, y3, c);
    DrawLine(x3, y3, x1, y1, c);
}

void Grid::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, char c)
{
    auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
    auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) SetTile(i, ny, c); };
    
    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next2:
        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                     // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else   	   	  t1x += signx1;
            if (i<dx1) i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
    next4:

        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        drawline(minx, maxx, y);   										
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y>y3) return;
    }
}

bool Grid::IsOutOfBounds(int x, int y)
{
    return x < 0 || x >= width || y < 0 || y >= height;
}

bool Grid::IsCollision(int x, int y, char c)
{
    return tiles[y][x] == c;
}

int Grid::GetWidth() const { return width; }
int Grid::GetHeight() const { return height; }
int Grid::GetMidWidth() const { return midWidth; }
int Grid::GetMidHeight() const { return midHeight; }

Grid grid{0,0};