#include "Photo.h"

void Photo::reset()
{
    if(!canUndo()) return;

    delete currentImage;
    currentImage = m_originalImage;
    m_undoSlider = 0;
}

bool Photo::canUndo() const
{
    return !m_undoStack.empty() && m_undoSlider > 0;
}

bool Photo::canRedo() const
{
    return (!m_undoStack.empty()) && (m_undoSlider < m_undoStack.size() - 1);
}

void Photo::pushChanges()
{
    if (*currentImage == *m_originalImage) return;

    m_undoStack.push_back(currentImage);
    ++m_undoSlider;
}


Photo::Photo(Image *originalIMG)
{
    if (originalIMG == nullptr)
    {
        throw std::invalid_argument("The provided image pointer is null!");
    }

    this->m_originalImage = this->currentImage = originalIMG;
}

void Photo::undo()
{
    if(!canUndo()) return;

    this->currentImage = m_undoStack[--m_undoSlider];
}

Photo::~Photo()
{
    if (m_originalImage) delete m_originalImage;
    if (currentImage) delete currentImage;

    for (size_t i = 0; i < m_undoStack.size(); ++i) {
        if (m_undoStack[i]) delete m_undoStack[i];
    }
}

void Photo::redo()
{
    if (!canRedo()) return;

    currentImage = m_undoStack[++m_undoSlider];
}
