
#include "scroll.hxx"

#include "ui.hxx"
#include "theme.hxx"
#include "listitem.hxx"

#include <stdio.h>

using namespace Avtk;

Scroll::Scroll( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ ),
  newChildCr( false ),
  childCr( 0x0 ),
  scrollX_( 0 ),
  scrollY_( 0 )
{
}

void Scroll::set( Widget* child )
{
  Group::add( child );
  newChildCr = true;
}

void Scroll::draw( cairo_t* cr )
{
  if( Widget::visible() )
  {
    cairo_save( cr );
    
    if( newChildCr && children.size() )
    {
      Widget* child = children.at(0);
      
      // create a cairo context for the child widgets size
      if( childCr )
      {
        printf("destroying existing childCr surface + context\n");
        cairo_surface_destroy( cairo_get_target( childCr ) );
        cairo_destroy( childCr );
        printf("done.\n");
      }
      
      cairo_surface_t* surface = cairo_surface_create_similar(
                                    cairo_get_target( cr ),
                                    CAIRO_CONTENT_COLOR_ALPHA,
                                    child->w(), child->h() );
      
      if (!surface)
      {
        fprintf(stderr, "failed to create child cairo surface\n");
      }
      
      if (!(childCr = cairo_create(surface)) )
      {
        fprintf(stderr, "failed to create child cairo context\n");
      }
      
      //cairo_surface_write_to_png( cairo_get_target( childCr ), "childCr.png" );
      newChildCr = false;
      
      // flag child to be drawn
      redrawChild_ = true;
    }
    
    if( redrawChild_ )
    {
      printf("cairo redrawing child in scroll group\n");
      redrawChild( cr );
    }
    cairo_save( cr );
    cairo_surface_t* s = cairo_get_target( childCr );
    cairo_surface_flush( s );
    
    // clip the Scroll context, to draw only what will be shown
    cairo_rectangle( cr, x_, y_, w_, h_ );
    cairo_clip( cr );
    
    // paint to the x_,y_ co-ord of the scroll window
    cairo_set_source_surface( cr, s, x_ - scrollX_, y_ + scrollY_ );
    cairo_paint( cr );
    
    cairo_restore( cr );
    
    // draw box / scroll bars
    roundedBox(cr, x_, y_, w_, h_, theme_->cornerRadius_ );
    theme_->color( cr, FG );
    cairo_set_line_width(cr, 0.5);
    cairo_stroke( cr );
    
    cairo_restore( cr );
  }
}

void Scroll::vertical( float v )
{
  scrollY_ = v * h_;
  ui->redraw();
}

void Scroll::horizontal( float v )
{
  scrollX_ = (1-v) * w_;
  ui->redraw();
}

void Scroll::redrawChild( cairo_t* cr )
{
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255., 24/255., 24/255. );
  cairo_fill( cr );
  
  // draw the widget on the childCr cairo_t*
  Group::draw( childCr );
  
  redrawChild_ = false;
}
