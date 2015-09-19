
#include "maximizer.hxx"
#include "theme.hxx"

using namespace Avtk;

Maximizer::Maximizer( Avtk::UI* ui, int x_, int y_, int w_, int h_,
    std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Maximizer()\n");
}

void Maximizer::draw( cairo_t* cr )
{
  cairo_save( cr );

  cairo_rectangle( cr, x_, y_, w_, h_);
  cairo_clip( cr );
  theme_->color( cr, BG, 0.4 );
  cairo_rectangle( cr, x_, y_, w_, h_);
  cairo_fill( cr );
  
  // S distortion curve
  float distort = value();
  cairo_move_to( cr, x_ , y_ + h_ );
  cairo_curve_to( cr, x_ +  w_ * distort,      y_+h_,  // control point 1
                  x_ + w_ - (w_ * distort),       y_,  // control point 2
                  x_ + w_,                       y_ ); // end of curve

  cairo_line_to ( cr, x_ + w_, y_ + h_ );
  cairo_close_path(cr);

  cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 0.21 );
  cairo_fill_preserve(cr);
  cairo_set_line_width(cr, 2.0);
  cairo_set_source_rgba( cr, 0 / 255.f, 153 / 255.f , 255 / 255.f , 1 );
  cairo_stroke(cr);
  
  // left top down bar
  cairo_save( cr );
  theme_->color( cr, FG, 0.4 );
  cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 0.9);
  cairo_set_line_width( cr, 10 );
  cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
  cairo_move_to( cr, x_ + w_*0.25, y_+2 );
  cairo_line_to( cr, x_ + w_*0.25, y_+ (h_/2.)*tone );
  cairo_stroke( cr );
  
  // right bottom up bar
  cairo_move_to( cr, x_ + w_*0.75, y_+h_ - 2 );
  cairo_line_to( cr, x_ + w_*0.75, y_+h_ - (h_/2.)*tone );
  cairo_stroke( cr );
  cairo_restore( cr );

  // outline
  cairo_set_line_width( cr, 1 );
  cairo_rectangle( cr, x_, y_, w_, h_ );
  cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
  cairo_stroke( cr );

  cairo_restore( cr );
}

