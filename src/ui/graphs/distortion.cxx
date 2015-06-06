
#include "distortion.hxx"
#include "theme.hxx"

using namespace Avtk;

Distortion::Distortion( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  dragMode( DM_DRAG_VERTICAL );
  scrollDisable = false;
}

void Distortion::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  cairo_move_to( cr, x_, y_ + h_ );
  //cairo_line_to( cr, x_+w_*0.1, y_+h_*0.85 - (h_*0.7*dryWet) );
  //cairo_line_to( cr, x_+w_*0.3+w_*0.7*size, y_+(h_*0.99) );
  
  if( true ) // active 
    theme_->color( cr, HIGHLIGHT, 0.2 );
  else
    theme_->color( cr, BG_DARK, 0.5 );
  cairo_fill_preserve( cr );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  cairo_stroke( cr );

  // fancy graph
  {
    int x = x_;
    int y = y_;
    int w = w_;
    int h = h_;
    
    cairo_save( cr );
    
    cairo_translate(cr, w/1.9, h*3.1333/4 );
    cairo_rotate(cr, value() * 3.1415 );
    cairo_translate(cr, -w/1.9, -h*3.1333/4 );
    /*
    cairo_move_to(cr, w/2, h/2 );
    cairo_translate(cr, w/2, h/2 );
    cairo_rotate(cr, _drive * 3.1415 * 2 );
    */
    // draw square, using curves
    cairo_move_to( cr, x + w     / 4, y + h     / 4 );
    
    
    float tmp = value() * w/6.5;
    
    // top left across
    cairo_curve_to(cr, x + w * 1.5 / 4, y + h * 1 / 4 + tmp * 2,
                       x + w * 2 / 4  , y + h * 1 / 4 - tmp,
                       x + w * 3 / 4  , y + h * 1 / 4 );
    
    // top right down
    cairo_curve_to(cr, x + w * 3 / 4 - tmp * 2, y + h * 1.5 / 4,
                       x + w * 3 / 4 + tmp    , y + h * 2 / 4,
                       x + w * 3 / 4          , y + h * 3 / 4 );
    
    // bottom right across
    cairo_curve_to(cr, x + w * 2.5 / 4, y + h * 3 / 4  - tmp * 2,
                       x + w * 1.5 / 4  , y + h * 3 / 4 + tmp,
                       x + w * 1   / 4  , y + h * 3 / 4 );
    
    // bottom left up
    cairo_curve_to(cr, x + w * 1 / 4 + tmp * 2, y + h * 2.5 / 4,
                       x + w * 1 / 4 - tmp    , y + h * 1.5 / 4,
                       x + w * 1 / 4          , y + h * 1.0 / 4 );
    
    
    cairo_close_path( cr );
    cairo_set_source_rgba( cr,  0 / 255.f,   155 / 255.f ,  255 / 255.f , 0.2 );
    cairo_set_line_width(cr, 1.5);
    cairo_fill_preserve( cr );
    cairo_set_source_rgba( cr,  0 / 255.f,   155 / 255.f ,  255 / 255.f , 0.8 );
    cairo_stroke( cr );
    
    cairo_restore( cr );
  }
 
  // outline
  cairo_rectangle( cr, x_, y_, w_, h_ );
  theme_->color( cr, FG );
  cairo_stroke( cr );

  cairo_restore( cr );
}

