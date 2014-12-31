
#include "scroll.hxx"

#include "ui.hxx"
#include "theme.hxx"
#include "slider.hxx"
#include "listitem.hxx"

#include <stdio.h>

using namespace Avtk;

#define AVTK_SCROLL_BAR_SIZE 15

Scroll::Scroll( Avtk::UI* ui, int x_, int y_, int w_, int h_, std::string label_) :
  Group( ui, x_, y_, w_, h_, label_ ),
  newChildCr( false ),
  childCr( 0x0 ),
  scrollX_( 0 ),
  scrollY_( 0 ),
  scrollV_( false ),
  scrollH_( false ),
  
  vSlider( new Avtk::Slider( ui, x_ + w_ - AVTK_SCROLL_BAR_SIZE, y_, AVTK_SCROLL_BAR_SIZE, h_, "Scroll VSlider") ),
  hSlider( new Avtk::Slider( ui, x_, y_ - w_ - AVTK_SCROLL_BAR_SIZE, w_, AVTK_SCROLL_BAR_SIZE, "Scroll HSlider") )
{
  // deal with sliders: they're a unique case where they're owned by the scroll,
  // but they are *not* part of the group. First remove the widget from the UI
  vSlider->parent()->remove( vSlider );
  hSlider->parent()->remove( hSlider );
  // then set the callbacks to the scoll movement
  vSlider->callback   = staticSliderCB;
  vSlider->callbackUD = this;
  hSlider->callback   = staticSliderCB;
  hSlider->callbackUD = this;
}

void Scroll::sliderCB( Widget* w )
{
  if( w == vSlider )
  {
    vertical( w->value() );
  }
  if( w == hSlider )
  {
    horizontal( w->value() );
  }
}

void Scroll::childResize( Widget* w )
{
  set( w );
}

void Scroll::set( Widget* child )
{
  Group::add( child );
  newChildCr = true;
  
  // set child to draw at 0,0 of the childCr
  child->x( 0 );
  child->y( 0 );
  
  redrawChild_ = true;
  
  if( child->h() > h_ )
  {
    // child is bigger than our vertical size:
    scrollV_ = true;
    scrollVamount = child->h() - h_;
  }
  else
  {
    // set the childs size to the scroll area
    child->h( h_ - AVTK_SCROLL_BAR_SIZE );
    scrollV_ = false;
    scrollY_ = 0;
  }
  
  if( child->w() > w_ )
  {
    // child is bigger than our vertical size:
    scrollH_ = true;
    scrollHamount = child->w() - w_;
    if( scrollV_ )
      scrollHamount = child->w() - (w_ - 4);
    //printf("Scroll::set() scrollHamount %i\n",scrollHamount);
  }
  else
  {
    // set the childs size to the scroll area
    child->w( w_ - AVTK_SCROLL_BAR_SIZE );
    scrollH_ = false;
    scrollX_ = 0;
  }
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
        //printf("destroying existing childCr surface + context\n");
        cairo_surface_destroy( cairo_get_target( childCr ) );
        cairo_destroy( childCr );
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
      //printf("cairo redrawing child in scroll group\n");
      redrawChild( cr );
    }
    
    // clip the Scroll context, to draw only what will be shown
    cairo_rectangle( cr, x_, y_, w_, h_ );
    cairo_clip( cr );
    
    // paint to the x_,y_ co-ord of the scroll window
    cairo_surface_t* s = cairo_get_target( childCr );
    cairo_set_source_surface( cr, s, x_ + scrollX_, y_ + scrollY_ );
    cairo_paint( cr );
    
    // draw box / scroll bars
    roundedBox(cr, x_, y_, w_, h_, theme_->cornerRadius_ );
    theme_->color( cr, FG );
    cairo_set_line_width(cr, 0.5);
    cairo_stroke( cr );
    
    if( scrollV_ )
    {
      vSlider->draw( cr );
    }
    if( scrollH_ )
    {
      //hSlider->draw( cr );
    }
    
    cairo_restore( cr );
  }
}

void Scroll::vertical( float v )
{
  if( scrollV_ ) // child->h() > h()
  {
    scrollY_ = -( (1-v)*scrollVamount);
    vSlider->value( v );
    ui->redraw();
  }
}

void Scroll::horizontal( float v )
{
  if( scrollH_ ) // child->w() > w()
  {
    scrollX_ = -( v*scrollHamount );
    ui->redraw();
    //printf("scrollH_ %i, value %f, scrollHamount %i\n",scrollX_,v, scrollHamount);
  }
}

int Scroll::handle( const PuglEvent* event )
{
  
  int ret = vSlider->handle( event );
  if( ret )
  {
    printf("vSlider returning from handle\n");
    return ret;
  }
  
  /*
  bool handleThisEvent = false;
  if( event->type == PUGL_BUTTON_PRESS ||
      event->type == PUGL_BUTTON_RELEASE )
  {
    if( touches( event->button.x, event->button.y ) )
      handleThisEvent = true;
  }
  if( event->type == PUGL_SCROLL )
  {
    if( touches( event->scroll.x, event->scroll.y ) )
      handleThisEvent = true;
  }
  
  if( handleThisEvent )
  {
    printf("Scroll handle(), type %i\n", event->type);
    int ret = vSlider->handle( event );
    if( ret )
    {
      printf("vSlider returning from handle\n");
      return ret;
    }
    ret = hSlider->handle( event );
    if( ret )
    {
      printf("hSlider returning from handle\n");
      return ret;
    }
    return Group::handle( event );
  }
  */
}

void Scroll::redrawChild( cairo_t* cr )
{
  cairo_save( cr );
  
  /// clear the screen
  cairo_rectangle( cr, 0, 0, w_, h_ );
  cairo_set_source_rgb( cr, 24/255., 24/255., 24/255. );
  cairo_fill( cr );
  
  // draw the widget on the childCr cairo_t*
  Group::draw( childCr );
  
  cairo_surface_t* s = cairo_get_target( childCr );
  cairo_surface_flush( s );
  
  redrawChild_ = false;
  
  cairo_restore( cr );
}

Scroll::~Scroll()
{
  delete vSlider;
  delete hSlider;
}
