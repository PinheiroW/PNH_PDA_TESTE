class PNH_MapUI extends UIScriptedMenu {

    private ref array<ref MarkerInfo> m_ClientMarkers = new ref array<ref MarkerInfo>;
    private ref array<ref MarkerInfo> m_ServerMarkers = new ref array<ref MarkerInfo>;
    private ref array<ref MarkerInfo> m_CustomServerMarkers = new ref array<ref MarkerInfo>;
    private bool m_Initialized = false;
    private bool m_IsMenuOpen;
    private string m_AddressPort;

    MapWidget m_MapWidget;
    protected ref PNH_MarkerList m_Adapter; // Ligado ao novo Componente
    private Widget m_PanelEditDialog;
    ImageWidget m_MapFakeBg;
    protected ref PNH_MapEdit m_EditDialog; // Ligado ao novo Componente
    private TextWidget m_CoordinatesText;
    private bool m_ShowingPlayerCoords = false;

    private ref MarkerInfo m_LastEditedMarker;
    private string m_LastEditedMarkerName;
    private int m_MarkerSuffix = 1;

    private MultilineTextWidget m_info_text;

    void PNH_MapUI()
    {
    }

    void ~PNH_MapUI() {
        PPEffects.SetBlurMenu( 0 );
        GetGame().GetUIManager().Back();
        g_Game.GetUIManager().ShowCursor(true);
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetMission().GetHud().Show( true );

        // TODO Fase 3: Mover isto para o PNH_MapStorage
        g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_ClientMarkers);
        g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_CustomServerMarkers, m_AddressPort);
        g_Game.GetClientMarkersCache().SaveCache();
    }

    override Widget Init() {
        g_Game.ShowAllHidden3dMarkers();
        if (!m_Initialized) {
            m_AddressPort = g_Game.GetConnectAddressPort();

            layoutRoot = GetGame().GetWorkspace().CreateWidgets("PNH_Tablet/GUI/VPP/Layouts/VPPMapMenu.layout");
            m_MapWidget = MapWidget.Cast( layoutRoot.FindAnyWidget( "Map_Widget" ) );
            WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_MapWidget, this, "MapDoubleClick" );

            Widget panelListFrame = layoutRoot.FindAnyWidget( "panel_list_frame" );
            m_Adapter = new PNH_MarkerList(this, panelListFrame); // Chama o nosso PNH_MarkerList

            m_PanelEditDialog = layoutRoot.FindAnyWidget( "panel_editdialog" );
            m_MapFakeBg = ImageWidget.Cast(m_PanelEditDialog.FindAnyWidget("map_fakebg"));
            m_MapFakeBg.LoadImageFile(0, "PNH_Tablet\\GUI\\VPP\\Textures\\map_blurred.paa");

            m_CoordinatesText = TextWidget.Cast(layoutRoot.FindAnyWidget("coordinates_text"));

            ImageWidget infoImage  = ImageWidget.Cast(layoutRoot.FindAnyWidget( "info_image" ));
            infoImage.LoadImageFile(0, "set:vpp_map_ui image:icon_info");

            m_info_text = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget( "info_text" ));
            string tooltip = "Duplo Clique no mapa para adicionar marcação";
            if (!g_Game.CanUse3DMarkers())
                tooltip += "\nEste servidor tem os hologramas 3D desativados!";

            m_info_text.SetText(tooltip);

            ReloadMarkers();
            m_Initialized = true;
        }
        return layoutRoot;
    }

    override void Update(float timeslice) {
        super.Update(timeslice);

        autoptr Widget underCursor = GetWidgetUnderCursor();
        if (underCursor == null) return;

        if (EditBoxWidget.Cast(underCursor)) {
            g_Game.SetKeyboardBusy(true);
        } else {
            g_Game.SetKeyboardBusy(false);
        }

        DisplayPlayerPosition();

        string text;
        vector coords;
        if (underCursor.GetName() == "Map_Widget") {
            coords = GetMapClickPos();
            text = string.Format("Cursor X/Y: %1 / %2", Math.Round(coords[0]), Math.Round(coords[2]));
            m_CoordinatesText.Show(true);
            m_CoordinatesText.SetText(text);
            m_CoordinatesText.SetColor(ARGB(255,255,255,255));
            m_ShowingPlayerCoords = false;
        } else if (!m_ShowingPlayerCoords && !g_Game.OwnPositionMarkerDisabled()) {
            coords = GetGame().GetPlayer().GetPosition();
            text = string.Format("Meu X/Y: %1 / %2", Math.Round(coords[0]), Math.Round(coords[2]));
            m_CoordinatesText.Show(true);
            m_CoordinatesText.SetText(text);
            m_CoordinatesText.SetColor(ARGB(255,255,255,0));
            m_ShowingPlayerCoords = true;
        } else if (g_Game.OwnPositionMarkerDisabled()) {
            m_CoordinatesText.Show(false);
        }
    }

    bool IsMenuOpen() {
        return m_IsMenuOpen;
    }

    void SetMenuOpen(bool isMenuOpen) {
        m_IsMenuOpen = isMenuOpen;
        if (m_IsMenuOpen)
            PPEffects.SetBlurMenu( 0.5 );
        else
            PPEffects.SetBlurMenu( 0 );
    }

    void ReloadMarkers() {
        ref ClientMarkersCache cache = g_Game.GetClientMarkersCache();
        if (cache) {
            cache.LoadCache(); 
            m_ClientMarkers = cache.GetCachedArray();
            m_CustomServerMarkers = cache.GetCachedArray(m_AddressPort);
            DisplayClientMarkers();
            DisplayServerMarkers();
        }
    }

    private void BuildClientHeaderItem(PNH_HeaderItem headerItem, ref array<ref MarkerInfo> markers) {
        headerItem.Clear();
        for(int i = 0; i < markers.Count(); i++) {
            MarkerInfo marker = markers.Get(i);
            if (marker.IsActive()) {
                m_MapWidget.AddUserMark(marker.GetPosition(), marker.GetName(), VPPMapMenu.VectorToARGB(marker.GetColor()), marker.GetIconPath());
            }
            headerItem.AddListItem(new PNH_ListItem(headerItem, marker, i));

            if (g_Game.CanUse3DMarkers()) {
                g_Game.AddClient3dMarker( new VPP3DMarker(marker.GetName(), marker.GetIconPath(), marker.GetPosition(), marker.Is3DActive()) );
            }
        }
    }

    void DisplayClientMarkers() {
        m_MapWidget.ClearUserMarks();
        DisplayPlayerPosition();
        g_Game.SetClient3dMarkers(NULL); 
        g_Game.SetServer3dMarkers(NULL); 
        PNH_HeaderItem headerItem = m_Adapter.GetClientMarkersHeaderItem();
        BuildClientHeaderItem(headerItem, m_ClientMarkers);
        headerItem = m_Adapter.GetCustomServerMarkersHeaderItem();
        BuildClientHeaderItem(headerItem, m_CustomServerMarkers);
        m_Adapter.UpdateContent();
    }

    void DisplayServerMarkers() {
        m_ServerMarkers = g_Game.GetServerMarkers();
        ref array<ref VPP3DMarker> marker3d = new array<ref VPP3DMarker>;
        if (m_ServerMarkers != NULL) {
            PNH_HeaderItem headerItem = m_Adapter.GetServerMarkersHeaderItem();
            headerItem.Clear();
            for(int i = 0; i < m_ServerMarkers.Count(); i++) {
                MarkerInfo marker = m_ServerMarkers.Get(i);
                if(marker.IsActive()) {
                    m_MapWidget.AddUserMark(marker.GetPosition(), marker.GetName(), VPPMapMenu.VectorToARGB(marker.GetColor()), marker.GetIconPath());
                }
                if(marker.Is3DActive() && !marker.Is3DForcedDisabled()) {
                    marker3d.Insert(new VPP3DMarker(marker.GetName(),marker.GetIconPath(),marker.GetPosition(), true));
                }
                headerItem.AddListItem(new PNH_ListItem(headerItem, marker, i));
            }
            g_Game.SetServer3dMarkers(marker3d);
        }
        m_Adapter.UpdateContent();
    }

    void DisplayPlayerPosition() {
        if (!g_Game.OwnPositionMarkerDisabled()) {
            m_MapWidget.AddUserMark(GetGame().GetPlayer().GetPosition(), "Eu", ARGB(255,255,255,0), "PNH_Tablet\\GUI\\VPP\\Textures\\CustomMapIcons\\waypointeditor_CA.paa");
        }
    }

    vector GetMapClickPos() {
        vector world_pos,ScreenToMap,dir,from,to;

        dir = GetGame().GetPointerDirection();
        from = GetGame().GetCurrentCameraPosition();
        to = from + ( dir * 1000 );

        world_pos = GetGame().GetScreenPos( to );
        ScreenToMap = m_MapWidget.ScreenToMap( world_pos );
        return ScreenToMap;
    }

    void MapDoubleClick(Widget w, int x, int y, int button) {
        if (button == MouseState.LEFT) {
            int index;
            string suffix;
            if (m_LastEditedMarker) {
                string name = m_LastEditedMarkerName;
                if (m_MarkerSuffix > 0) {
                    suffix = " [" + m_MarkerSuffix + "]";
                    name += suffix;
                }
                m_MarkerSuffix++;
                index = m_ClientMarkers.Insert( new MarkerInfo(name, m_LastEditedMarker.GetIconPath(), m_LastEditedMarker.GetColor(), GetMapClickPos(), true, true) );
            } else {
                index = m_ClientMarkers.Insert( new MarkerInfo("Novo Ponto", "dz\\gear\\navigation\\data\\map_border_cross_ca.paa", Vector(255,255,255), GetMapClickPos(), true, true) );
            }
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_ClientMarkers);
            g_Game.GetClientMarkersCache().SaveCache(); 
            ReloadMarkers();
            ShowDialog(index, false, true, suffix);
        } else if (button == MouseState.RIGHT) {
            RemoveMarker(GetMapClickPos());
        }
    }

    void RemoveMarkerExact(vector pos, bool isCustomServer) {
        string addressPort = "";
        if (isCustomServer) {
            addressPort = m_AddressPort;
        }
        g_Game.GetClientMarkersCache().RemoveCachedMarker(pos, addressPort);
        g_Game.GetClientMarkersCache().SaveCache(); 
        ReloadMarkers();
    }

    void RemoveMarker(vector relativePos) {
        float minDistance = 86, distance;
        ref MarkerInfo nearestMarker;
        bool isCustomServerMarker = false;
        foreach(MarkerInfo clientMarker : m_ClientMarkers) {
            distance = vector.Distance(relativePos, clientMarker.GetPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestMarker = clientMarker;
            }
        }
        foreach(MarkerInfo marker : m_CustomServerMarkers) {
            distance = vector.Distance(relativePos, marker.GetPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestMarker = marker;
                isCustomServerMarker = true;
            }
        }
        if (nearestMarker) {
            RemoveMarkerExact(nearestMarker.GetPosition(), isCustomServerMarker);
        }
    }

    void EditMarkerVisibility(array<int> indexes, bool show2D, bool show3D, bool isClientMarker, bool isCustomServerMarker) {
        array<ref MarkerInfo> markers;
        foreach(int index : indexes) {
            if (isClientMarker) {
                markers = m_ClientMarkers;
            } else if (isCustomServerMarker) {
                markers = m_CustomServerMarkers;
            } else {
                markers = m_ServerMarkers;
            }
            MarkerInfo marker = markers.Get(index);
            marker.SetActive(show2D);
            marker.Set3DActive(show3D);
        }

        if (isClientMarker || isCustomServerMarker) {
            string addressPort = "";
            if (isCustomServerMarker) {
                addressPort = m_AddressPort;
            }
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(markers, addressPort);
            g_Game.GetClientMarkersCache().SaveCache(); 
        }
        ReloadMarkers(); 
    }

    void EditMarkerAttributes(int index, string name, string markerSuffix, string iconPath, vector color, bool wasCustomServer, bool isCustomServer) {
        ref array<ref MarkerInfo> markers;
        if (wasCustomServer) {
            markers = m_CustomServerMarkers;
        } else {
            markers = m_ClientMarkers;
        }
        m_LastEditedMarker = markers.Get(index);
        m_LastEditedMarker.SetName(name);
        m_LastEditedMarker.SetIconPath(iconPath);
        m_LastEditedMarker.SetColor(color);
        string markerName = name;
        if (markerSuffix) {
            markerName.Replace(markerSuffix, "");
        }
        if (!m_LastEditedMarkerName || m_LastEditedMarkerName != markerName) {
            m_MarkerSuffix = 1;
            m_LastEditedMarkerName = markerName;
        }

        if (wasCustomServer && !isCustomServer) {
            g_Game.GetClientMarkersCache().RemoveCachedMarker(m_LastEditedMarker.GetPosition(), m_AddressPort);
            m_ClientMarkers.Insert(m_LastEditedMarker);
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_ClientMarkers);
        } else if (!wasCustomServer && isCustomServer) {
            g_Game.GetClientMarkersCache().RemoveCachedMarker(m_LastEditedMarker.GetPosition());
            m_CustomServerMarkers.Insert(m_LastEditedMarker);
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_CustomServerMarkers, m_AddressPort);
        } else if (isCustomServer && wasCustomServer) {
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_CustomServerMarkers, m_AddressPort);
        } else if (!isCustomServer && !wasCustomServer) {
            g_Game.GetClientMarkersCache().UpdateCachedMarkerInfos(m_ClientMarkers);
        }
        g_Game.GetClientMarkersCache().SaveCache(); 
        ReloadMarkers(); 
    }

    void ShowDialog(int markerIndex, bool isCustomServer, bool isNewMarker, string markerSuffix = "") {
        g_Game.HideAll3dMarkers();
        if (!m_EditDialog) {
            m_EditDialog = new PNH_MapEdit(this, m_PanelEditDialog); // Abre o teu novo Menu PNH!
        }
        m_MapWidget.Show(false);
        m_MapFakeBg.Show(true);
        m_PanelEditDialog.Show(true);
        ref array<ref MarkerInfo> markers;
        if (isCustomServer) {
            markers = m_CustomServerMarkers;
        } else {
            markers = m_ClientMarkers;
        }
        MarkerInfo marker = markers.Get(markerIndex);
        m_EditDialog.SetMarker(markerIndex, marker, isCustomServer, isNewMarker, markerSuffix);
    }

    void HideDialog() {
        g_Game.ShowAllHidden3dMarkers();
        if (m_EditDialog) {
            m_PanelEditDialog.Show(false);
            m_MapWidget.Show(true);
            m_MapFakeBg.Show(false);
        }
    }
};