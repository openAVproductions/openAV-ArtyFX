
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

  cairo_restore( cr );
}

