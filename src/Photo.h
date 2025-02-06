#ifndef _PHOTO_H
#define _PHOTO_H

#include "Image.h"
#include <vector>

class Photo {
public:
    Image *currentImage{nullptr};

    Photo(Image *originalIMG);
    ~Photo();
    void reset();
    bool canUndo() const;
    bool canRedo() const;
    void pushChanges();
    void undo();
    void redo();

private:
    Image *m_originalImage{nullptr};
    std::vector<Image*> m_undoStack{};
    int m_undoSlider{};

};


#endif //_PHOTO_H
