class PNH_IconInfo
{
    string Name;
    string Path;

    void PNH_IconInfo(string name, string path)
    {
        Name = name;
        Path = path;
    }
}

class PNH_IconManager
{
    static ref array<ref PNH_IconInfo> m_Icons;

    static void InitIcons()
    {
        if (m_Icons) return; // Se a memória já tiver os ícones, não cria de novo!
        
        m_Icons = new array<ref PNH_IconInfo>;
        
        // Ícones Nativos do DayZ
        m_Icons.Insert(new PNH_IconInfo("Cross", "dz\\gear\\navigation\\data\\map_border_cross_ca.paa"));
        
        // Ícones PNH Tablet (Militares e Civis)
        m_Icons.Insert(new PNH_IconInfo("Me", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\waypointeditor_CA.paa"));
        m_Icons.Insert(new PNH_IconInfo("Marker", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\waypoint_CA.paa"));
        m_Icons.Insert(new PNH_IconInfo("Completed", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\waypointcompleted_CA.paa"));
        m_Icons.Insert(new PNH_IconInfo("Tent", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\tent.paa"));
        m_Icons.Insert(new PNH_IconInfo("Car", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\car.paa"));
        m_Icons.Insert(new PNH_IconInfo("Truck", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\iconTruck_ca.paa"));
        m_Icons.Insert(new PNH_IconInfo("Heli", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\pictureParachute_ca.paa"));
        m_Icons.Insert(new PNH_IconInfo("House", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\house.paa"));
        m_Icons.Insert(new PNH_IconInfo("Shop", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\shopping.paa"));
        m_Icons.Insert(new PNH_IconInfo("Hospital", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\hospital.paa"));
        m_Icons.Insert(new PNH_IconInfo("Police", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\police.paa"));
        m_Icons.Insert(new PNH_IconInfo("FireStation", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\firestation.paa"));
        m_Icons.Insert(new PNH_IconInfo("Water", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\waterpump.paa"));
        m_Icons.Insert(new PNH_IconInfo("DeerStand", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\deerstand.paa"));
        m_Icons.Insert(new PNH_IconInfo("Bar", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\bar.paa"));
        m_Icons.Insert(new PNH_IconInfo("Church", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\church.paa"));
        m_Icons.Insert(new PNH_IconInfo("Fuel", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\fuelstation.paa"));
        m_Icons.Insert(new PNH_IconInfo("RadioTower", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\radiotower.paa"));
        m_Icons.Insert(new PNH_IconInfo("Lighthouse", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\lighthouse.paa"));
        m_Icons.Insert(new PNH_IconInfo("Warning", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\warning_CA.paa"));
        m_Icons.Insert(new PNH_IconInfo("Objective", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\objective_CA.paa"));
        m_Icons.Insert(new PNH_IconInfo("Unknown", "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\unknown_CA.paa"));
    }

    static array<ref PNH_IconInfo> GetIcons()
    {
        InitIcons();
        return m_Icons;
    }
}