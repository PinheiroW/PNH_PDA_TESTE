// Ficheiro: PNH_Tablet/Scripts/3_Game/PNH_Tablet/Storage/PNH_MapStorage.c

class PNH_MapStorage {
    private static const string SETTINGS_DIR  = "$profile:PNH_Tablet\\";
    private static const string CLIENT_FILE   = SETTINGS_DIR + "ClientMarkers.json";
    
    private static ref PNH_MapConfig m_Config;

    static void Load() {
        if (!m_Config) m_Config = new PNH_MapConfig();
        if (!FileExist(SETTINGS_DIR)) MakeDirectory(SETTINGS_DIR);

        if (FileExist(CLIENT_FILE)) {
            JsonFileLoader<PNH_MapConfig>.JsonLoadFile(CLIENT_FILE, m_Config);
        }
    }

    static void Save() {
        if (!m_Config) return;
        JsonFileLoader<PNH_MapConfig>.JsonSaveFile(CLIENT_FILE, m_Config);
    }

    static array<ref PNH_MarkerData> GetMarkers() {
        if (!m_Config) Load();
        return m_Config.Markers;
    }

    static void AddMarker(ref PNH_MarkerData marker) {
        if (!m_Config) Load();
        m_Config.Markers.Insert(marker);
        Save();
    }

    static void RemoveMarkerAtPos(vector pos) {
        if (!m_Config) return;
        for (int i = 0; i < m_Config.Markers.Count(); i++) {
            if (vector.Distance(m_Config.Markers[i].GetPosition(), pos) < 1.0) {
                m_Config.Markers.Remove(i);
                Save();
                return;
            }
        }
    }
}