class PNH_ColorManager
{
    static ref array<vector> m_Colors;

    static void InitColors()
    {
        if (m_Colors) return;
        m_Colors = new array<vector>;
        
        m_Colors.Insert(Vector(255,255,255)); // White
        m_Colors.Insert(Vector(255,0,0));     // Red
        m_Colors.Insert(Vector(0,255,0));     // Green
        m_Colors.Insert(Vector(0,0,255));     // Blue
        m_Colors.Insert(Vector(255,255,0));   // Yellow
        m_Colors.Insert(Vector(255,0,255));   // Magenta
        m_Colors.Insert(Vector(0,255,255));   // Cyan
        m_Colors.Insert(Vector(0,0,0));       // Black
        m_Colors.Insert(Vector(255,128,0));   // Orange
        m_Colors.Insert(Vector(128,128,128)); // Grey
    }

    static array<vector> GetColors()
    {
        InitColors();
        return m_Colors;
    }
}