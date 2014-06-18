/*
 * Author: Harry van Haaren 2014
 *         harryhaaren@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */


#ifndef AVTK_WIDENER_H
#define AVTK_WIDENER_H

#include <FL/Fl_Widget.H>
#include <FL/Fl_Slider.H>

#include <FL/Fl_Menu_Item.H>

#include <valarray>
#include <string>

namespace Avtk
{
  
class Widener : public Fl_Slider
{
  public:
    Widener(int _x, int _y, int _w, int _h, const char *_label = 0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      label = _label;
      
      mouseClickedX = 0;
      mouseClickedY = 0;
      mouseClicked = false;
      
      active = true;
      highlight = false;
      invert = false;
    }
    
    bool active;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    int mouseClickedX;
    int mouseClickedY;
    bool mouseClicked;
    bool mouseRightClicked;
    
    bool invert;
    
    float volume;
    float feedback;
    
    void setInvert( float i )
    {
      invert = false;
      
      if ( i > 0.5 )
        invert = true;
      
      redraw();
    }
    
    void setFeedback(float f)
    {
      feedback = f;
      redraw();
    }
    
    void setVolume(float e)
    {
      volume = e;
      redraw();
    }
    
    float getVolume()
    {
      return volume;
    }
    
    void setActive(bool a)
    {
      active = a;
      redraw();
    }
    
    bool getActive()
    {
      return active;
    }
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        
        cairo_set_line_width(cr, 1.5);
        
        
        // fill background
        cairo_rectangle( cr, x, y, w, h);
        cairo_set_source_rgb( cr, 28 / 255.f,  28 / 255.f ,  28 / 255.f  );
        cairo_fill_preserve( cr );
        cairo_clip( cr );
        
        
        // set up dashed lines, 1 px off, 1 px on
        double dashes[1];
        dashes[0] = 2.0;
        
        cairo_set_dash ( cr, dashes, 1, 0.0);
        cairo_set_line_width( cr, 1.0);
        
        // loop over each 2nd line, drawing dots
        cairo_set_line_width(cr, 1.0);
        cairo_set_source_rgb(cr, 0.4,0.4,0.4);
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x + ((w / 4.f)*i), y );
          cairo_line_to( cr, x + ((w / 4.f)*i), y + h );
        }
        for ( int i = 0; i < 4; i++ )
        {
          cairo_move_to( cr, x    , y + ((h / 4.f)*i) );
          cairo_line_to( cr, x + w, y + ((h / 4.f)*i) );
        }
        
        cairo_set_source_rgba( cr,  66 / 255.f,  66 / 255.f ,  66 / 255.f , 0.5 );
        cairo_stroke(cr);
        cairo_set_dash ( cr, dashes, 0, 0.0);
        
        
        // curved waveshape
        /*
        float distort = value();
        cairo_move_to( cr, x , y + h );
        cairo_curve_to( cr, x +  w * distort,      y+h,    // control point 1
                        x + w - (w * distort),       y,    // control point 2
                        x + w,                       y );  // end of curve 1, start curve 2
        
        cairo_line_to ( cr, x + w, y + h );
        cairo_close_path(cr);
        
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
        cairo_fill_preserve(cr);
        cairo_set_line_width(cr, 2.0);
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
        cairo_stroke(cr);
        */
        
        
        cairo_set_line_width(cr, 7.3);
        cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
        
        if ( !invert )
        {
          // vertical line up
          cairo_move_to( cr, x + w/2, y + h - h * 0.15 );
          cairo_line_to( cr, x + w/2, y + h - h * 0.5  );
          cairo_set_source_rgba(cr, 0.4,0.4,0.4, 0.7);
        }
        else
        {
          cairo_move_to( cr, x + w/2 - 13, y + h - h * 0.2 );
          cairo_line_to( cr, x + w/2 + 13, y + h - h * 0.45  );
          //cairo_set_source_rgba(cr, 0.4,0.4,0.4, 0.7);
          cairo_set_source_rgba( cr, 255 / 255.f, 81 / 255.f , 0 / 255.f , 0.8 );
          cairo_stroke( cr );
          
          cairo_move_to( cr,  x + w/2, y + h - h * 0.325 );
          //cairo_set_source_rgba(cr, 0,0,0, 0.0);
          cairo_stroke(cr);
          
          cairo_arc( cr, x + w/2, y + h - h * 0.325, 14, 0, 6.28);
          //cairo_set_source_rgba(cr, 0.4,0.4,0.4, 0.7);
        }
        cairo_stroke(cr);
        
        // arc to left
        printf("value %f\n", value() );
        cairo_arc_negative( cr, x + w/2, y + h - h * 0.15 , w/2 - 15, -3.14/2.f, -3.14/2.f -3.14/2.f*value() );
        cairo_move_to( cr, x + w/2, y + h - h * 0.62 );
        cairo_arc         ( cr, x + w/2, y + h - h * 0.15 , w/2 - 15, -3.14/2.f, -3.14/2.f*( 1-value() ) );
        cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.8 );
        cairo_stroke(cr);
        
        
        // stroke outline
        cairo_rectangle(cr, x+1, y+1, w-2, h-2);
        cairo_set_source_rgba( cr,  126 / 255.f,  126 / 255.f ,  126 / 255.f , 0.8 );
        cairo_set_line_width(cr, 1.0);
        cairo_stroke( cr );
        
        
        if ( !active )
        {
          // big grey X
          cairo_set_line_width(cr, 20.0);
          cairo_set_source_rgba(cr, 0.4,0.4,0.4, 0.7);
          
          cairo_move_to( cr, x + (3 * w / 4.f), y + ( h / 4.f ) );
          cairo_line_to( cr, x + (w / 4.f), y + ( 3 *h / 4.f ) );
          
          cairo_move_to( cr, x + (w / 4.f), y + ( h / 4.f ) );
          cairo_line_to( cr, x + (3 * w / 4.f), y + ( 3 *h / 4.f ) );
          cairo_set_line_cap ( cr, CAIRO_LINE_CAP_BUTT);
          cairo_stroke( cr );
        }
        
        cairo_restore( cr );
      }
    }
    
    void resize(int X, int Y, int W, int H)
    {
      Fl_Widget::resize(X,Y,W,H);
      x = X;
      y = Y;
      w = W;
      h = H;
      redraw();
    }
    
    int handle(int event)
    {
      switch(event)
      {
        case FL_PUSH:
          highlight = 0;
          mouseRightClicked = false;
          
          mouseClickedX = Fl::event_x();
          mouseClickedY = Fl::event_y();
          
          if ( Fl::event_button() == FL_RIGHT_MOUSE )
          {
            active = !active;
            redraw();
            mouseRightClicked = true;
            do_callback();
          }
          return 1;
        case FL_DRAG:
          {
            if ( Fl::event_state(FL_BUTTON1) )
            {
              if ( mouseClicked == false ) // catch the "click" event
              {
                mouseClickedX = Fl::event_x();
                mouseClickedY = Fl::event_y();
                mouseClicked = true;
              }
              
              float deltaX = mouseClickedX - Fl::event_x();
              float deltaY = mouseClickedY - Fl::event_y();
              
              float valX = value() ;
              valX -= deltaX / 100.f;
              float valY = volume;
              valY += deltaY / 100.f;
              
              if ( valX > 1.0 ) valX = 1.0;
              if ( valX < 0.0 ) valX = 0.0;
              
              if ( valY > 1.0 ) valY = 1.0;
              if ( valY < 0.0 ) valY = 0.0;
              
              //handle_drag( value + deltaY );
              set_value( valX );
              volume = valY;
              
              mouseClickedX = Fl::event_x();
              mouseClickedY = Fl::event_y();
              redraw();
              do_callback();
            }
          }
          return 1;
        case FL_RELEASE:
          mouseRightClicked = false;
          if (highlight) {
            highlight = 0;
            redraw();
            do_callback();
          }
          mouseClicked = false;
          return 1;
        case FL_SHORTCUT:
          if ( test_shortcut() )
          {
            do_callback();
            return 1;
          }
          return 0;
        default:
          return Fl_Widget::handle(event);
      }
      
      return 0;
    }
    
  private:
};

} // Avtk

#endif // AVTK_WIDENER_H
