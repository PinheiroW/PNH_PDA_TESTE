modded class VPPMapMenu extends UIScriptedMenu
{
    private bool m_IsPNHInitialized = false;

    override Widget Init()
    {
        if (!m_Initialized)
        {
            // Aponta para o layout da carcaça que ajustamos com as divisões de tela
            layoutRoot = GetGame().GetWorkspace().CreateWidgets("PNH_Tablet/PDA/data/KT_PDA_VPPMap.layout");
            
            m_MapWidget     = MapWidget.Cast(layoutRoot.FindAnyWidget("Map_Widget"));
            m_PanelMarkers  = layoutRoot.FindAnyWidget("panel_markers");
            m_PanelEditList = layoutRoot.FindAnyWidget("panel_editdialog");
            
            // Lista de Marcadores
            m_MarkersListBox = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("Marker_ListBox"));
            
            m_Initialized = true;
        }
        
        return layoutRoot;
    }

    // Força o carregamento dos ícones da pasta interna agora
    override void OnShow()
    {
        super.OnShow();
        Print("[PNH_Tablet] Sistema de Mapa Interno Iniciado.");
    }
}