#include "SFMLCamera.hpp"

Camera::Camera(const sf::Vector2u &winSize, const Bounds2D &bounds) : m_winSize(winSize)
{
    setToBounds(bounds);
}

const sf::View &Camera::getView() const
{
    return m_view;
}

void Camera::updateView()
{
    m_view.setCenter(m_viewPos + m_viewSize/2.f);
    m_view.setSize(m_viewSize);
}

void Camera::handleEvent(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::MouseWheelScrolled:
        if(event.mouseWheelScroll.delta > 0)
            zoom(0.8);
        else
            zoom(1.25f);
        break;
    case sf::Event::MouseMoved:
        m_mouseScreenPos.x = event.mouseMove.x;
        m_mouseScreenPos.y = event.mouseMove.y;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            pan();
        m_lastScreenMousePos = m_mouseScreenPos;
        break;
    case sf::Event::Resized:
        resize(event.size.width, event.size.height);
        break;
    default:
        break;
    }
}

void Camera::update(float dt){}

void Camera::setToBounds(const Bounds2D &bounds)
{
    float winAspectRatio = (float)m_winSize.x / (float)m_winSize.y;
    float width = (bounds.max_x - bounds.min_x);
    float height = (bounds.max_y - bounds.min_y);
    float boundAspectRatio = width / height;

    if(winAspectRatio < boundAspectRatio)
    {
        m_viewPos.x = bounds.min_x;
        m_viewSize.x = bounds.max_x - bounds.min_x;
        m_viewSize.y = m_viewSize.x / winAspectRatio;
        float y_offset = (m_viewSize.y - height) / 2.f;
        m_viewPos.y = bounds.min_y - y_offset;
    }
    else
    {
        m_viewPos.y = bounds.min_y;
        m_viewSize.y = bounds.max_y - bounds.min_y;
        m_viewSize.x = m_viewSize.y * winAspectRatio;
        float x_offset = (m_viewSize.x - width) / 2.f;
        m_viewPos.x = bounds.min_x - x_offset;
    }

    updateView();
}

sf::Vector2f Camera::screenToWorld(const sf::Vector2i &screenPos)
{
    sf::Vector2f relPos;
    relPos.x = (float)screenPos.x / (float)m_winSize.x;
    relPos.y = (float)screenPos.y / (float)m_winSize.y;

    sf::Vector2f worldPos;
    worldPos.x = (relPos.x * m_viewSize.x) + m_viewPos.x;
    worldPos.y = (relPos.y * m_viewSize.y) + m_viewPos.y;

    return worldPos;
}

void Camera::zoom(float f)
{
    sf::Vector2f oldMousePos = screenToWorld(m_mouseScreenPos);

    sf::Vector2f newSize = m_viewSize * f;
    m_viewSize = newSize;

    sf::Vector2f updatedMousePos = screenToWorld(m_mouseScreenPos);
    
    sf::Vector2f offset = oldMousePos - updatedMousePos;
    m_viewPos += offset;

    updateView();
}

void Camera::scale(float f)
{
    sf::Vector2f oldSz = m_viewSize;
    m_viewSize *= f;
    sf::Vector2f offset = (m_viewSize - oldSz)/2.f;
    m_viewPos -= offset;
}

void Camera::resize(unsigned int newWidth, unsigned int newHeight)
{
    float widthRatio = (float)newWidth/(float)m_winSize.x;
    float heightRatio = (float)newHeight/(float)m_winSize.y;

    m_winSize.x = newWidth;
    m_winSize.y = newHeight;

    m_viewSize.x *= widthRatio;
    m_viewSize.y *= heightRatio;

    updateView();
}

void Camera::pan()
{
    sf::Vector2f pos = screenToWorld(m_mouseScreenPos);
    sf::Vector2f lastPos = screenToWorld(m_lastScreenMousePos);
    sf::Vector2f offset = pos - lastPos;
    m_viewPos -= offset;

    updateView();
}