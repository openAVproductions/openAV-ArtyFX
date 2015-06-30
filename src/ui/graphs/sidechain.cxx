
#include "sidechain.hxx"
#include "theme.hxx"

using namespace Avtk;

Sidechain::Sidechain( Avtk::UI* ui, int x_, int y_, int w_, int h_,
    std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{ 
  time = 0.5;
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Sidechain()\n");
}

void Sidechain::draw( cairo_t* cr )
{
  cairo_save( cr );
  
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_clip( cr );
  theme_->color( cr, BG, 0.4 );
  cairo_rectangle( cr, x_, y_, w_, h_); 
  cairo_fill( cr );

  /*
  cairo_set_line_width( cr, 18 );
  cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
  theme_->color( cr, HIGHLIGHT, 0.8 );
  */
      
  // imported from NTK sidechain widget
  {
    int x = x_;
    int y = y_;
    int w = w_;
    int h = h_;
    
    
    // draw threshold / ducked line
    float tmpX = x+w*0.750-(w*0.5*(1-threshold));
    cairo_move_to( cr, tmpX, y  );
    cairo_line_to( cr, tmpX + value()*reduceAmount*(w*0.5), y + h / 2 );
    cairo_line_to( cr, x + w * 0.750 - (w * 0.5 * (1-threshold)), y + h );
    
    cairo_line_to( cr, x + w , y + h );
    cairo_line_to( cr, x + w , y );
    cairo_close_path( cr );
    cairo_set_source_rgba( cr, 0 / 255.f, 0x77/255.f, 255 / 255.f , 0.21 );
    cairo_fill_preserve( cr );
    cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
    cairo_stroke( cr );
    
    // value() => arrow
    cairo_move_to( cr, x+w*0.00 * value(), y + h * 0.4 - (h*0.1*value()) );
    cairo_line_to( cr, x+w*0.65 * value(), y + h * 0.4 );
    cairo_line_to( cr, x+w*0.1  + w * 0.65 * value(), y + h * 0.5 );
    cairo_line_to( cr, x+w*0.65 * value(), y + h * 0.6 );
    cairo_line_to( cr, x+w*0.00 * value(), y + h * 0.6 + (h*0.1*value()) );
    cairo_close_path( cr );
    cairo_set_source_rgba( cr, 1.0, 0.318, 0.f , 0.21 );
    cairo_fill_preserve( cr );
    cairo_set_source_rgba( cr, 1.0, 0.318, 0.f , 1 );
    cairo_stroke( cr );
    
    
    // time - horizontal line
    cairo_move_to( cr, x    , y + h * 0.25 + h/2 * time );
    cairo_line_to( cr, x + w, y + h * 0.25 + h/2 * time );
    cairo_set_source_rgba( cr, 1.0, 0.0, 0.f , 1 );
    cairo_stroke( cr );
  }

  // outline
  cairo_set_line_width( cr, 1 );
  cairo_rectangle( cr, x_, y_, w_, h_ );
  cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
  cairo_stroke( cr );

  cairo_restore( cr );
}

