#include "Photo.h"
#include <stdexcept>

Photo::Photo(Image *originalIMG)
{
    if (originalIMG == nullptr)
        throw std::invalid_argument("The provided image pointer is null!");

    m_originalImage = originalIMG;
    currentImage    = new Image(*originalIMG);
    m_undoSlider    = -1;
}

Photo::~Photo()
{
    cleanUp();
}

bool Photo::canUndo() const
{
    return !m_undoStack.empty();
}

bool Photo::canRedo() const
{
    return m_undoSlider < static_cast<int>(m_undoStack.size()) - 1;
}

void Photo::pushChanges()
{
    while (static_cast<int>(m_undoStack.size()) - 1 > m_undoSlider)
    {
        delete m_undoStack.back();
        m_undoStack.pop_back();
    }

    m_undoStack.push_back(new Image(*currentImage));
    m_undoSlider = static_cast<int>(m_undoStack.size()) - 1;
}

void Photo::undo()
{
    if (!canUndo()) return;

    delete currentImage;

    if (m_undoSlider > 0)
    {
        --m_undoSlider;
        currentImage = new Image(*m_undoStack[m_undoSlider]);
    }
    else
    {
        m_undoSlider = -1;
        currentImage = new Image(*m_originalImage);
    }
}

void Photo::redo()
{
    if (!canRedo()) return;

    delete currentImage;
    ++m_undoSlider;
    currentImage = new Image(*m_undoStack[m_undoSlider]);
}

void Photo::reset()
{
    for (Image *img : m_undoStack) delete img;
    m_undoStack.clear();
    m_undoSlider = -1;

    delete currentImage;
    currentImage = new Image(*m_originalImage);
}

void Photo::cleanUp()
{
    delete m_originalImage;
    m_originalImage = nullptr;

    delete currentImage;
    currentImage = nullptr;

    for (Image *img : m_undoStack) delete img;
    m_undoStack.clear();
    m_undoSlider = -1;
}