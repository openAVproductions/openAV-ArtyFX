
#include "filter.hxx"
#include "theme.hxx"

using namespace Avtk;

Filter::Filter( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{ 
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Filter()\n");
}

void Filter::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_clip( cr );
  theme_->color( cr, BG, 0.4 );
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_fill( cr );

  
  cairo_set_line_width( cr, 18 );
  cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  
  // imported from NTK filter widget
  {
    int x = x_;
    int y = y_;
    int w = w_;
    int h = h_;
    
    float freq = 0;
    int graphType = 0;
    if ( value() < 0.45 )
    {
      graphType = 0;
      freq = value()*2.f;
    }
    else if ( value() > 0.55 )
    {
      graphType = 1;
      freq = (value()-0.5)*2.f;
    }
    else
    {
      graphType = 2;
    }
    
    switch( graphType )
    {
      case 0 :
        {
          // draw the cutoff line:
          // move to bottom left, draw line to middle left
          cairo_move_to( cr, x , y + h );
          cairo_line_to( cr, x , y + (h*0.47));
          
          float cutoff = 0.1 + freq * 0.85;
          
          // Curve
          cairo_curve_to( cr, x + w * cutoff    , y+(h*0.5)  ,   // control point 1
                              x + w * cutoff    , y+(h * 0.3),   // control point 2
                              x + w * cutoff + 5, y+ h       );  // end of curve 1
          
          cairo_close_path(cr);
          
          cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
          cairo_fill_preserve(cr);
          cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
          cairo_set_line_width(cr, 1.5);
          cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
          cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
          cairo_stroke( cr );
        }
            break;
      case 1:
          {
            // draw the cutoff line:
            float cutoff = 0.95 - (freq*0.8);
            
            // move to bottom right
            cairo_move_to( cr, x + w, y + h );
            cairo_line_to( cr, x + w, y + (h*0.47));
            
            // Curve
            cairo_curve_to( cr, x + w - (w*cutoff)    , y+(h*0.5)  ,   // control point 1
                                x + w - (w*cutoff)    , y+(h * 0.3),   // control point 2
                                x + w - (w*cutoff) - 5, y+ h      );   // end of curve 1
            
            cairo_close_path(cr);
            
            // stroke
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
            cairo_fill_preserve(cr);
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
            cairo_set_line_width(cr, 1.5);
            cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
            cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke( cr );
          }
          break;
      case 2:
          {
            // move to bottom right
            cairo_move_to( cr, x + w, y + h );
            cairo_line_to( cr, x + w, y + (h*0.47));
            cairo_line_to( cr, x    , y + (h*0.47));
            cairo_line_to( cr, x    , y + h       );
            cairo_close_path(cr);
            
            // stroke
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
            cairo_fill_preserve(cr);
            cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
            cairo_set_line_width(cr, 1.5);
            cairo_set_line_join( cr, CAIRO_LINE_JOIN_ROUND);
            cairo_set_line_cap ( cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke( cr );
          }
          break;
      default:
          break;
    }
  }

  cairo_restore( cr );
}

