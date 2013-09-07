/*
 * Author: Harry van Haaren 2013
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


#ifndef AVTK_MASHER_H
#define AVTK_MASHER_H

#include <FL/Fl_Slider.H>

namespace Avtk
{

class Masher : public Fl_Slider
{
  public:
    Masher(int _x, int _y, int _w, int _h, const char *_label =0):
        Fl_Slider(_x, _y, _w, _h, _label)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
      
      _duration = 0.5;
      _volume   = 1.0;
      _replace  = 1.0;
      
      active = true;
      
      label = _label;
      
      highlight = false;
      mouseOver = false;
    }
    
    void volume(float v){ _volume = v; redraw(); }
    void replace(float r){ _replace = r; redraw(); }
    void duration(float d){ _duration = d; redraw(); }
    
    bool getActive(){return active;}
    void setActive(bool a){active = a; redraw();}
    
    float _volume;
    float _duration;
    float _replace;
    
    bool active;
    
    bool mouseOver;
    bool highlight;
    int x, y, w, h;
    const char* label;
    
    void draw()
    {
      if (damage() & FL_DAMAGE_ALL)
      {
        cairo_t *cr = Fl::cairo_cc();
        
        cairo_save( cr );
        
        // graph
        cairo_rectangle( cr, x, y, w, h );
        cairo_set_source_rgb( cr,28 / 255.f,  28 / 255.f ,  28 / 255.f  );
        cairo_fill(cr);
        
        
        // set up dashed lines, 1 px off, 1 px on
        double dashes[1];
        dashes[0] = 2.0;
        
        cairo_set_dash ( cr, dashes, 1, 0.0);
        
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
        
        
        // check that its a new "segment" to redraw
        int newAngle = ( _duration * 4.9f);
        
        int xc = (w) / 2.f;
        int yc = (h) / 2.f;
        
        float angle = 0.f;
        
        if      ( newAngle == 0 )
          angle = 0.06;
        else if ( newAngle == 1 )
          angle = 0.125;
        else if ( newAngle <= 2 )
          angle = 0.25;
        else if ( newAngle <= 3 )
          angle = 0.5;
        else
          angle = 1;
        
        // replace: middle arc
        cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND );
        cairo_arc( cr, x + xc, y + yc, xc * 6.5 / 14, -(3.1415/2), _replace * 6.28 - (3.1415/2) );
        cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.8 );
        cairo_set_line_width(cr, 9);
        cairo_stroke(cr);
        
        if      ( newAngle == 0 )
          cairo_set_source_rgba (cr, 1.0, 0.0 , 0.0, 1 );
        else if ( newAngle == 1 )
          cairo_set_source_rgba (cr, 1.0, 0.48, 0.0, 1 );
        else if ( newAngle <= 2 )
          cairo_set_source_rgba (cr, 0.0, 1.0 , 0.0, 0.8 );
        else if ( newAngle <= 3 )
          cairo_set_source_rgba (cr, 0.0, 0.48, 1.0, 1 );
        else
          cairo_set_source_rgba (cr, 1.0, 0.0, 1.0, 0.7 );
        
        
        // duration : outside arc
        cairo_arc( cr, x + xc, y + yc, xc * 9.5 / 14, -(3.1415/2), angle * 6.28 - (3.1415/2) );
        cairo_set_line_width(cr, 11);
        cairo_stroke(cr);
        
        
        // volume: inside circle
        cairo_set_source_rgba(cr,0.3,0.3,0.3, 0.5);
        cairo_arc(cr, x + xc, y + yc, 25 * _volume, 0, 2 * 3.1415);
        cairo_set_line_width(cr, 4.2);
        cairo_fill_preserve(cr);
        cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.8 );
        cairo_set_line_width(cr, 2);
        cairo_stroke(cr);
        
        // stroke rim
        cairo_rectangle(cr, x, y, w, h);
        //cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
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
        
        draw_label();
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
      switch(event) {
        case FL_PUSH:
          
          highlight = 1;
          
          if ( Fl::event_button() == FL_RIGHT_MOUSE )
          {
            active = !active;
            redraw();
            do_callback();
          }
          return 1;
        case FL_DRAG: {
            int t = Fl::event_inside(this);
            if (t != highlight) {
              highlight = t;
              redraw();
            }
          }
          return 1;
        case FL_ENTER:
          mouseOver = true;
          redraw();
          return 1;
        case FL_LEAVE:
          mouseOver = false;
          redraw();
          return 1;
        case FL_RELEASE:
          if (highlight) {
            highlight = 0;
            redraw();
          }
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
    }
};

} // Avtk

#endif // AVTK_ADSR_H

