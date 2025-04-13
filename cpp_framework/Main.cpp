#include <iostream>
#include <string>
#include "Carriage.hpp"


struct SPoint {
  size_t X = 0;
  size_t Y = 0;
};

enum EAlign {
  EA_Left,
  EA_Center,
  EA_Right
};

struct SColor {
  EColor Text = EColor::EC_White;
  EColor Bg = EColor::EC_Black;
};


class AObject
{
public:
  size_t id = 0;
  std::string name = "";
  size_t seniorid = 0; 

  AObject() noexcept : id(reinterpret_cast<size_t>(this)) {}
};

class ADialog : public AObject
{
  // Key_Handler
  // Render
};


class ALabel : AObject
{
public:
  SPoint s_point; 
  size_t width = 0;
  size_t height = 0;
  std::string caption = "";
  SColor color;
  EAlign align;
    
};



int main(int argc, char** argv) 
{  
  ADialog main_d;
  ALabel label;
  

	return 0;
}

