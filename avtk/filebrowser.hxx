
#ifndef OPENAV_AVTK_FILE_BROWSER_HXX
#define OPENAV_AVTK_FILE_BROWSER_HXX

#include "group.hxx"

namespace Avtk
{

class Box;

/** FileBrowser
 * A generic file browsing / selection tool, that provides easy navigation of
 * the filesystem. Armed with hotkeys and shortcuts for power users, the user
 * experience is kept as simple as possible for lightning fast navigation.
 * 
 * This class provides more callbacks than an Avtk::Widget. 
 */
class FileBrowser : public Group
{
  public:
    FileBrowser( Avtk::UI* ui, int x, int y, int w, int h, std::string label);
    virtual ~FileBrowser(){}
    
    virtual void draw( cairo_t* cr );
    
    /// a callback that is called when the user *changes* the selection. This is
    /// not the "OK" clicked callback: this is called once per selected file.
    /// use case: Auditioning audio samples : play file on selectionChanged()
    void (*selectionChanged)(Avtk::Widget* w, void* userdata);
    void*  selectionChangedUD;
  
  private:
    Box* header;
    
};

};

#endif // OPENAV_AVTK_FILE_BROWSER_HXX
