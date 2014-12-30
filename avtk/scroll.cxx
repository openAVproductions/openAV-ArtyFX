
#include "scroll.hxx"

#include "ui.hxx"
#include "theme.hxx"
#include "listitem.hxx"

#include <stdio.h>

using namespace Avtk;

Scroll::Scroll( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ ),
  newChildCr( false ),
  childCr( 0x0 )
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
    
    if( true /*newChildCr*/ && children.size() )
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
      
      /*
      // draw box / scroll bars
      roundedBox(cr, 40, 40, 420, 420, 5 );
      cairo_set_source_rgb(cr, 1, 0 , 0);
      cairo_set_line_width(cr, 0.5);
      cairo_fill( cr );
      */
      
      children.at(0)->draw( childCr );
      
      //Group::draw( childCr );
      cairo_surface_write_to_png( cairo_get_target( childCr ), "childCr.png" );
    
      printf("cairo child surface / context created OK.\n");
      newChildCr = false;
    }
    
    printf("cairo redrawing scroll group\n");
    
    // have the group draw itself to the childCr
    
    
    cairo_save( cr );
    cairo_surface_t* s = cairo_get_target( childCr );
    cairo_surface_flush( s );
    cairo_move_to( cr, x_, y_ );
    cairo_set_source_surface( cr, s, x_, y_ );
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

