#include <wx/wx.h>
#include <wx/graphics.h>

class RoundedButton : public wxControl
{
public:
    RoundedButton(wxWindow* parent,
        wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
		const wxColour& bgColour = wxColour(0, 0, 0),
		const wxColour& textColour = wxColour(255, 255, 255),
        long style = wxBORDER_NONE,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr)
        : wxControl(parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE | wxTRANSPARENT_WINDOW, validator, name),
        m_label(label),
        m_backgroundColor(bgColour),
        m_textColor(textColour),
        m_cornerRadius(10),
        m_isHovered(false),
        m_isPressed(false)
    {
        // The wxTRANSPARENT_WINDOW style is now set in the base class constructor
        Bind(wxEVT_PAINT, &RoundedButton::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &RoundedButton::OnMouseDown, this);
        Bind(wxEVT_LEFT_UP, &RoundedButton::OnMouseUp, this);
        Bind(wxEVT_ENTER_WINDOW, &RoundedButton::OnMouseEnter, this);
        Bind(wxEVT_LEAVE_WINDOW, &RoundedButton::OnMouseLeave, this);
    }

    void SetBackgroundColor(const wxColour& color)
    {
        m_backgroundColor = color;
        Refresh();
    }
    void SetTextColor(const wxColour& color) { m_textColor = color; Refresh(); }
    void SetCornerRadius(int radius) { m_cornerRadius = radius; Refresh(); }
	wxString GetNewLabel() { return m_label; }

private:
    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        //dc.SetBackgroundMode(wxTRANSPARENT);
		dc.SetBrush(wxBrush(wxColour(0xffff00)));

        wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

        if (gc)
        {
            gc->SetPen(*wxTRANSPARENT_PEN);
            const wxRect rect = GetClientRect();
			//rect.SetColour(m_backgroundColor);
            

            // Draw button
            wxColour buttonColor = m_backgroundColor;
            if (m_isPressed)
                buttonColor = buttonColor.ChangeLightness(80);
            else if (m_isHovered)
                buttonColor = buttonColor.ChangeLightness(110);
            else
				buttonColor = buttonColor.ChangeLightness(90);

            gc->SetBrush(wxBrush(buttonColor));
            //gc->DrawRoundedRectangle(0, 0, rect.width, rect.height, m_cornerRadius);
            gc->DrawRectangle(0, 0, rect.width, rect.height);
            // Draw text
            gc->SetFont(GetFont(), m_textColor);
            wxDouble textWidth, textHeight;
            gc->GetTextExtent(m_label, &textWidth, &textHeight);
            gc->DrawText(m_label, (rect.width - textWidth) / 2, (rect.height - textHeight) / 2);

            delete gc;
        }
    }

    void OnMouseEnter(wxMouseEvent& event) { m_isHovered = true; Refresh(); }
    void OnMouseLeave(wxMouseEvent& event) { m_isHovered = false; m_isPressed = false; Refresh(); }
    void OnMouseDown(wxMouseEvent& event) { m_isPressed = true; Refresh(); }
    void OnMouseUp(wxMouseEvent& event)
    {
        m_isPressed = false;
        Refresh();
        wxCommandEvent clickEvent(wxEVT_BUTTON, GetId());
        clickEvent.SetEventObject(this);
        ProcessEvent(clickEvent);
    }

    wxString m_label;
    wxColour m_backgroundColor;
    wxColour m_textColor;
    int m_cornerRadius;
    bool m_isHovered;
    bool m_isPressed;
    
};