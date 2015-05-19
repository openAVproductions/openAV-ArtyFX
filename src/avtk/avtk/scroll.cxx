
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
  setCtrlZoom_( false ),
  
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
  
  // show top left corner or scroll window
  vertical  ( 1 );
  horizontal( 0 );
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
    
    if( childCr )
    {
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
    }
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
      hSlider->draw( cr );
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

void Scroll::setCtrlZoom( bool zoom )
{
  setCtrlZoom_ = zoom;
}

int Scroll::handle( const PuglEvent* event )
{
  // handle slider, so slider-click is responeded to
  if( vSlider->handle( event ) )
    return 1;
  
  bool handle = false;
  
  // check if the scroll event is in scroll area; if yes scroll action
  if( event->type == PUGL_SCROLL )
  {
    if( touches( event->scroll.x, event->scroll.y ) )
    {
      // not control pressed
      if( !(((PuglEventScroll*)event)->state & PUGL_MOD_CTRL) )
      {
        // shift scrolls horizontal
        if( !(((PuglEventScroll*)event)->state & PUGL_MOD_SHIFT) )
        {
          if( event->scroll.dy > 0 )
            vSlider->value( vSlider->value() + 0.1 );
          else
            vSlider->value( vSlider->value() - 0.1 );
          vertical( vSlider->value() );
        }
        else
        {
          if( event->scroll.dy > 0 )
            hSlider->value( hSlider->value() - 0.1 );
          else
            hSlider->value( hSlider->value() + 0.1 );
          horizontal( hSlider->value() );
        }
        
        ui->redraw( this );
        
        // return, eating event, so child group won't react
        return 1;
      }
      else
      {
        if( !setCtrlZoom_ )
        {
          handle = true;
        }
        else
        {
          // zoom on the child widget: aka, change its 
          if( children.size() )
          {
            Widget* w = children.at(0);
            float scale = 0.75;
            if( event->scroll.dy > 0 )
              scale = 1.5;
            
            int newW = w->w() * scale;
            int newH = w->h() * scale;
            
            if( newW > 2048 * 2 || newH > 2048 * 2 )
              return 1; // no more zooming: cairo_t context gets too big
            
            w->w( newW );
            w->h( newH );
            
            childResize( w );
            
            // handled
            return 1;
          }
        }
      }
    }
  }
  
  // create group event, so we can offset the mouse-click co-ord according to
  // the scroll position. This might seem a lot of work, but it allows for easy
  // mouse handling in the child widget, because the co-ords are normal
  if( event->type == PUGL_BUTTON_PRESS ||
      event->type == PUGL_BUTTON_RELEASE )
  {
    if( touches( event->button.x, event->button.y ) )
    {
      PuglEvent childEvent;
      // offset to new location
      offsetEvent( event, &childEvent );
      // pass event on to children
      if( Group::handle( &childEvent ) )
      {
        newChildCr = true;
        ui->redraw();
        return 1;
      }
    }
  }
  
  
  if( handle )
  {
    if( Group::handle( event ) )
    {
      newChildCr = true;
      ui->redraw();
      return 1;
    }
  }
  
  return 0;
}

void Scroll::redrawChild( cairo_t* cr )
{
  if( !childCr )
  {
    redrawChild_ = false;
    return;
  }
  cairo_save( cr );
  
  /*
  /// clear the screen
  cairo_rectangle( childCr, 0, 0, w_, h_ );
  cairo_set_source_rgb( childCr, 24/255., 24/255., 24/255. );
  cairo_fill( childCr );
  */
  
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

void Scroll::offsetEvent( const PuglEvent* inEvent, PuglEvent* outEvent )
{
  // copy inEvent to outEvent
  *outEvent = *inEvent;
  
  // adjust properties as needed
  if( outEvent->type == PUGL_SCROLL )
  {
    outEvent->scroll.x += ( x_ + scrollX_ );
    outEvent->scroll.y -= ( y_ + scrollY_ );
  }
  else if( outEvent->type == PUGL_BUTTON_PRESS ||
           outEvent->type == PUGL_BUTTON_RELEASE )
  {
    outEvent->button.x -= ( x_ + scrollX_ );
    outEvent->button.y -= ( y_ + scrollY_ );
  }
  else
  {
    printf("%s, event type not handled!\n", __PRETTY_FUNCTION__ );
  }
}
