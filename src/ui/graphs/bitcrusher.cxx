
#include "bitcrusher.hxx"
#include "theme.hxx"

using namespace Avtk;

Bitcrusher::Bitcrusher( Avtk::UI* ui, int x_, int y_, int w_, int h_,
    std::string label_) :
  Widget( ui, x_, y_, w_, h_, label_ )
{ 
  //dragMode( DM_DRAG_VERTICAL );
  //scrollDisable = false;
  //printf("Avtk UI Bitcrusher()\n");
}

void Bitcrusher::draw( cairo_t* cr )
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
  
  // imported from NTK bitcrusher widget
  {
    int x = x_;
    int y = y_;
    int xSize = w_;
    int ySize = h_;
    
    bool active = true;
    float bitcrush = 0.125 + (1-value()) * 0.75;
    
    // move to bottom left
    cairo_move_to( cr, x_, y_ + h_ );
    
    // draw steps for bitcrush
    int prevY = y + ySize;
    for ( int i = 0; i < 16; i++)
    {
      int currY = y + ySize - (ySize * i/(bitcrush*16.f));
      int currX = x + xSize * i/(bitcrush*16.f);
      if ( currY > y + ySize || currX > x + xSize )
        break;
      
      cairo_line_to ( cr, currX, prevY );
      cairo_line_to ( cr, currX, currY );
      prevY = currY;
    }
    
    cairo_line_to ( cr, x + xSize, prevY );
    cairo_line_to ( cr, x + xSize, y + ySize );
    cairo_close_path( cr );
    
    if ( active )
      cairo_set_source_rgba( cr, 0/255.f, 153/255.f, 255/255.f, 0.2 );
    //else
      //setColour(cr, COLOUR_GREY_1, 0.2 );
    cairo_close_path(cr);
    cairo_fill_preserve(cr);
    
    // stroke cutoff line
    cairo_set_line_width( cr, 1.5);
    if ( active )
      cairo_set_source_rgba( cr, 0/255.f, 153/255.f, 255/255.f, 0.8 );
    //else
    //  setColour(cr, COLOUR_GREY_1 );
    cairo_stroke( cr );
    
    // outline
    cairo_rectangle( cr, x_, y_, w_, h_ );
    cairo_set_source_rgb( cr, 0.72, 0.72, 0.72 );
    cairo_stroke( cr );
  }

  cairo_restore( cr );
}

