#include <wx/wx.h>
#include <wx/graphics.h>

class roundedRectangle {
public:
    roundedRectangle(wxPanel* panel, wxWindowID id, const wxSize& size, const wxPoint& position, int radius)
        : m_panel(panel), m_id(id), m_size(size), m_position(position), m_radius(radius) {}

    void Draw(wxPaintDC& dc) {
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawRoundedRectangle(m_position, m_size, m_radius);
    }

private:
    wxPanel* m_panel;
    wxWindowID m_id;
    wxSize m_size;
    wxPoint m_position;
    int m_radius;
};

