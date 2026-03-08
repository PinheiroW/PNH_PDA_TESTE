// Ficheiro: PNH_Tablet/Scripts/3_Game/PNH_Tablet/Storage/PNH_MarkerData.c

class PNH_MarkerData {
    private string m_Name;
    private string m_IconPath;
    private vector m_Color;
    private vector m_Position;
    private bool   m_IsActive;
    private bool   m_Is3DActive;
    
    void PNH_MarkerData(string name, string path, vector color, vector pos, bool active = true, bool active3d = true) {
        m_Name       = name;
        m_IconPath   = path;
        m_Color      = color;
        m_Position   = pos;
        m_IsActive   = active;
        m_Is3DActive = active3d;
    }

    // Getters
    string GetName()      { return m_Name; }
    string GetIconPath()  { return m_IconPath; }
    vector GetColor()     { return m_Color; }
    vector GetPosition()  { return m_Position; }
    bool   IsActive()     { return m_IsActive; }
    bool   Is3DActive()   { return m_Is3DActive; }

    // Setters
    void SetName(string val)     { m_Name = val; }
    void SetIconPath(string val) { m_IconPath = val; }
    void SetColor(vector val)    { m_Color = val; }
    void SetActive(bool val)     { m_IsActive = val; }
    void Set3DActive(bool val)   { m_Is3DActive = val; }
}

class PNH_MapConfig {
    ref array<ref PNH_MarkerData> Markers;

    void PNH_MapConfig() {
        Markers = new array<ref PNH_MarkerData>;
    }
}