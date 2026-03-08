class PNH_MapEdit extends ScriptedWidgetEventHandler {

    VPPMapMenu m_Menu; // Na Fase 3, isto vai apontar para o PNH_MapUI
    ref MarkerInfo m_Marker; // Mais tarde, isto apontará para PNH_MarkerData
    int m_MarkerIndex;
    bool m_IsCustomServerMarker = false;
    string m_MarkerSuffix;

    Widget m_Root;
    EditBoxWidget m_EditBoxName;
    Widget m_EditBoxNameOverlay;
    Widget m_PanelColor;
    Widget m_PanelIcon;
    ButtonWidget m_ButtonDelete;
    ButtonWidget m_ButtonOK;
    Widget m_PanelClose;
    ButtonWidget m_ButtonClose;
    CheckBoxWidget m_CheckboxCustomServer;

    // Aponta para os nossos novos Adapters da Fase 1!
    ref IconListAdapter m_IconListAdapter; 
    ref ColorListAdapter m_ColorListAdapter;

    void PNH_MapEdit(VPPMapMenu menu, Widget root) {
        m_Menu = menu;
        // Caminho já atualizado para o teu layout
        m_Root = GetGame().GetWorkspace().CreateWidgets( "PNH_Tablet/GUI/VPP/Layouts/EditDialog.layout", root );
        m_EditBoxName = EditBoxWidget.Cast(m_Root.FindAnyWidget( "editbox_name" ));
        m_EditBoxNameOverlay = m_Root.FindAnyWidget( "editbox_name_overlay" );
        m_EditBoxNameOverlay.Show(false);
        m_PanelColor = m_Root.FindAnyWidget( "panel_color" );
        m_PanelIcon = m_Root.FindAnyWidget( "panel_icon" );
        m_ButtonDelete = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_delete" ));
        m_ButtonOK = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_ok" ));
        m_PanelClose = m_Root.FindAnyWidget( "panel_close" );
        m_ButtonClose = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_close" ));
        m_CheckboxCustomServer = CheckBoxWidget.Cast(m_Root.FindAnyWidget( "checkbox_customserver" ));

        m_IconListAdapter = new IconListAdapter(m_PanelIcon);
        // O ColorListAdapter original da tua Fase 1 tem de receber este Menu como argumento!
        m_ColorListAdapter = new ColorListAdapter(this, m_PanelColor); 

        m_Root.SetHandler(this);
    }

    void SetMarker(int markerIndex, MarkerInfo marker, bool isCustomServer, bool isNewMarker, string markerSuffix) {
        m_MarkerIndex = markerIndex;
        m_MarkerSuffix = markerSuffix;
        m_IsCustomServerMarker = isCustomServer;
        m_CheckboxCustomServer.SetChecked(m_IsCustomServerMarker);
        m_Marker = marker;
        m_EditBoxName.SetText(marker.GetName());
        m_EditBoxNameOverlay.Show(isNewMarker);
        m_IconListAdapter.SetSelectedIcon(marker.GetIconPath());
        m_IconListAdapter.SetSelectedColor(marker.GetColor());
        m_ColorListAdapter.SetSelectedColor(marker.GetColor());
    }

    // Esta é a função que é ativada pelo teu ColorListAdapter para pintar o ícone no momento do clique!
    void SetIconColor(vector color) {
        m_IconListAdapter.SetSelectedColor(color);
    }

    override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
        if (button == MouseState.LEFT) {
            if (w == m_PanelClose || w == m_ButtonClose) {
                m_Menu.HideDialog();
                return true;
            } else if (w == m_ButtonDelete) {
                m_Menu.RemoveMarkerExact(m_Marker.GetPosition(), m_IsCustomServerMarker);
                m_Menu.HideDialog();
                return true;
            } else if (w == m_ButtonOK) {
                string name = m_EditBoxName.GetText();
                string iconPath = m_IconListAdapter.GetSelectedIcon();
                vector color = m_ColorListAdapter.GetSelectedColor();
                bool checked = m_CheckboxCustomServer.IsChecked();
                m_Menu.EditMarkerAttributes(m_MarkerIndex, name, m_MarkerSuffix, iconPath, color, m_IsCustomServerMarker, checked);
                m_Menu.HideDialog();
                return true;
            } else if (w == m_EditBoxNameOverlay) {
                m_EditBoxName.SetText("");
                m_EditBoxNameOverlay.Show(false);
                return false;
            }
        }
        return false;
    }
}