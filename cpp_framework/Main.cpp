#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include "Carriage.hpp"


size_t Cur_UI_Id = 0;

enum EKeys
{
  EK_ESC = 27,
  EK_UP = 72,
  EK_DOWN = 80,
  EK_LEFT = 75,
  EK_RIGHT = 77,
  EK_Enter = 13,
  EK_SPACE = 32,
  EK_TAB = 9
};

struct SPoint {
  size_t X = 0;
  size_t Y = 0;
};

struct SColor {
  EColor Text = EColor::EC_White;
  EColor Bg = EColor::EC_Black;
};


class ALabel
{
 public:
   size_t id = 0;
   size_t seniorid = 0;
   size_t width = 10;
   size_t height = 1;
   char bg_symbol = ' ';
   bool visible = true;

   std::string name = "";
   std::string caption = "";
    
   SPoint s_point;
   SColor color;   


   ALabel() noexcept : id(reinterpret_cast<size_t>(this)) {}
   // ~ALabel()

   void Create() {}

   void Render() 
   {
     AsCarriage::Set_Color(this->color.Text, this->color.Bg);
     char symbol = this->bg_symbol;  // убрать локализацию
     size_t i = 0, j = 0;

     for (size_t y = 0; y < this->height; y++)
     {
       for (size_t x = 0; x < this->width; x++)
       {
         AsCarriage::Set_Coord(x + this->s_point.X, y + this->s_point.Y);
         std::cout << symbol;
         AsCarriage::Set_Coord(x + this->s_point.X, y + this->s_point.Y);
         if (j < this->caption.size()) std::cout << this->caption[j++];
       }
     }

     AsCarriage::Set_Default();
   }   
  
};



class As_TbCursor : public AsCarriage
{
public:
  static SPoint min_p;
  static SPoint max_p;  
  static SColor color;

  static void Move(int _dx, int _dy)
  {   
     if (Is_Print_Access(X + _dx, Y +_dy))
      AsCarriage::Move(_dx, _dy);
  }

  static void Init(SPoint _max_point, SPoint _min_point, SColor _cursor_color)
  {
    As_TbCursor::min_p = _min_point;
    As_TbCursor::max_p = _max_point;
    As_TbCursor::color = _cursor_color;
  }
  
  static void Set_Coord(int _x, int _y)
  {    
    if (Is_Print_Access(_x, _y))
      AsCarriage::Set_Coord(_x, _y);
  }

  static bool Is_Print_Access(int _x, int _y)
  {
    return ((As_TbCursor::min_p.X <= _x and _x < As_TbCursor::max_p.X) and (As_TbCursor::min_p.Y <= _y and _y < As_TbCursor::max_p.Y));
    // надо учитывать длину текста
  }

  static SPoint Get_Coord()
  {
    return SPoint{ (size_t)As_TbCursor::X, (size_t)As_TbCursor::Y};
  }

};

SPoint As_TbCursor::min_p;
SPoint As_TbCursor::max_p;
SColor As_TbCursor::color;



class ATextBox  // поддерживает взаимодействие 
{
   public:
     size_t id = 0;              // первая буква должна быть заглавной
     size_t seniorid = 0;        // по умолчанию текущий диалог (ид диалога)
     size_t width = 10;
     size_t height = 1;
     size_t min_text_length = width * height;
     

     char bg_symbol = '_';
     bool visible = true;
     bool enabled = true;

     std::string name = "";
     std::string text = "";          // память должна динамически выделяться в зависимости от ширины     
                                     // надо переименовать в caption
     
     SPoint s_point;
     SColor color;
     // tab_order    // зависит от сеньора


     // ----------------------------------------------------------------------------------------------------          
     ATextBox() noexcept : id(reinterpret_cast<size_t>(this)) {}
     // ~ALabel()        // освобождение памяти текстовых полей     


     // ----------------------------------------------------------------------------------------------------
     void Render()
     {
       if (this->s_point.X == 0)       
         this->s_point.X = 1;

        AsCarriage::Set_Color(this->color.Text, this->color.Bg);        
        size_t i = 0, j = 0;        
               
        for (size_t y = 0; y < this->height; y++)
        {
          for (size_t x = 0; x < this->width; x++) 
          {
             AsCarriage::Set_Coord(x + this->s_point.X, y + this->s_point.Y);
             printf("%c", this->bg_symbol);
             AsCarriage::Set_Coord(x + this->s_point.X, y + this->s_point.Y);
             if (j < this->text.size()) 
               printf("%c", this->text[j++]);
          }
        }

        As_TbCursor::Set_Coord(this->s_point.X, this->s_point.Y);
        AsCarriage::Set_Default();
     }


     // ----------------------------------------------------------------------------------------------------
     size_t Get_Index_By_Cursor(SPoint _cursor)    
     {            
       return (_cursor.Y - this->s_point.Y) * this->width + (_cursor.X - this->s_point.X);
     }

     
     // ----------------------------------------------------------------------------------------------------
     SPoint Get_Cursor_By_Index(size_t _cur_index)
     {
        size_t y = (size_t)(_cur_index / this->width);
        return SPoint{ (_cur_index - y * this->width) + this->s_point.X, y + this->s_point.Y };
     }


     // ----------------------------------------------------------------------------------------------------
     void Key_Handler()
     {
       bool exit_interaction_entity = false;
       
       As_TbCursor::Init(SPoint{this->s_point.X + this->width, this->s_point.Y + this->height}, s_point, this->color);
       
       // позиция курсора по умолчанию
       size_t cur_index = this->text.size();

       if (cur_index >= min_text_length)
         cur_index = min_text_length - 1;
         
       while (not exit_interaction_entity) 
       {                  
         // визуализация курсора
         As_TbCursor::Set_Color(color.Bg, color.Text);                                    
         As_TbCursor::Set_Coord(this->Get_Cursor_By_Index(cur_index).X, this->Get_Cursor_By_Index(cur_index).Y);         
         printf("%c", (cur_index < this->text.size() ? this->text[cur_index] : this->bg_symbol));
         
         // получение сигнала                  
         EKeys key = (EKeys)_getch();        

         // возврат форматирования пред. символа
         As_TbCursor::Set_Color(color.Text, color.Bg);
         As_TbCursor::Set_Coord(this->Get_Cursor_By_Index(cur_index).X, this->Get_Cursor_By_Index(cur_index).Y);                               
         printf("%c", (cur_index < this->text.size()  ? this->text[cur_index] : this->bg_symbol));
         

         switch (key) {
           case EK_ESC:
             exit_interaction_entity = true;        
             break;
           case EK_Enter:
             exit_interaction_entity = true;
             break;
           case EK_TAB:
             exit_interaction_entity = true;
             break;

           // case EK_Backspace
           // ...

           //case   // символ для печати    рег. выражение
           // ...

           // case delete
           // ...   
   
           case EK_UP: 
             As_TbCursor::Move(0, -1);
             cur_index = this->Get_Index_By_Cursor(As_TbCursor::Get_Coord());
             break;

           case EK_DOWN:              
             As_TbCursor::Move(0, 1);              
             if (this->Get_Index_By_Cursor(As_TbCursor::Get_Coord()) > this->text.size())
               As_TbCursor::Move(0, -1);
             cur_index = this->Get_Index_By_Cursor(As_TbCursor::Get_Coord());
             break;

           case EK_LEFT:              
             if (cur_index - 1 <= this->text.size() and cur_index - 1 < min_text_length)    // перенести в проверку доступности печати
             {
               cur_index--;
               As_TbCursor::Move(-1, 0);
             }
             break;

           case EK_RIGHT:                           
             if (cur_index + 1 <= this->text.size() and cur_index + 1 < min_text_length)    // перенести в проверку доступности печати
             {
               cur_index++;
               As_TbCursor::Move(1, 0);
             }
             break;                    
         
           default:
           {         
             if (cur_index >= min_text_length - 1)
               break;

             if (key != 224)    // надо убрать после применения рег выражения
             {               
               if (cur_index < this->text.size())
                 this->text[cur_index] = (char)key;
                 // операция вставки
                 // ...
               else               
                 this->text += (char)key;
                                                                
               As_TbCursor::Set_Coord(this->Get_Cursor_By_Index(cur_index).X, this->Get_Cursor_By_Index(cur_index).Y);
               printf("%c", (cur_index < this->text.size() ? this->text[cur_index] : this->bg_symbol));

               cur_index++;
             }
                            
            }
         }         
       }     
       
       AsCarriage::Set_Default();
     }

     // ----------------------------------------------------------------------------------------------------
     void On_Hover()     // должно быть свойством
     {     
        AsCarriage::Set_Coord(this->s_point.X - 1, this->s_point.Y);
        printf("[");

        AsCarriage::Set_Coord(this->s_point.X + width, this->s_point.Y + height - 1);
        printf("]");
       
        this->Key_Handler();
     }

     // переключение на другой интерактивный компонент ()
     // ...

};


// ----------------------------------------------------- MAIN
int main(int argc, char** argv) 
{  
   // AsProgram::Main_Start();   
   
   ALabel l_1;
   ALabel l_2;

   ATextBox tb_1;
   ATextBox tb_2;
  
  
   //tb_1.s_point.X = 1;
   //tb_1.s_point.Y = 1;
   tb_1.color.Text = EC_Blue;
   tb_1.color.Bg = EC_White;
   tb_1.height = 6;
   tb_1.width = 10;   
   tb_1.min_text_length = tb_1.width * tb_1.height;
   
   
   // tb_1.text = "Hello, World!";
   tb_1.text = "Hel! 9";

   
   tb_1.Render();
   tb_1.On_Hover();


   return 0;
}

