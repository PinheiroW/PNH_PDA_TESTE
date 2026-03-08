class PNH_MarkerList extends ListAdapter {

    private ref array<ref PNH_HeaderItem> m_HeaderItems = new array<ref PNH_HeaderItem>;

    private ref PNH_HeaderItem m_ServerMarkersHeaderItem;
    private ref PNH_HeaderItem m_CustomServerMarkersHeaderItem;
    private ref PNH_HeaderItem m_ClientMarkersHeaderItem;

    VPPMapMenu m_Menu; // Mais tarde, na Fase 3, isto vai apontar para o PNH_MapUI

    void PNH_MarkerList(VPPMapMenu menu, Widget root) {
        super.init("PNH_Tablet/GUI/VPP/Layouts/List/VerticalList.layout", root);

        m_Menu = menu;

        m_ServerMarkersHeaderItem = new PNH_HeaderItem("Marcadores do Servidor");
        m_HeaderItems.Insert(m_ServerMarkersHeaderItem);
        m_CustomServerMarkersHeaderItem = new PNH_HeaderItem("Meus Marcadores Server");
        m_HeaderItems.Insert(m_CustomServerMarkersHeaderItem);
        m_ClientMarkersHeaderItem = new PNH_HeaderItem("Meus Marcadores Pessoais");
        m_HeaderItems.Insert(m_ClientMarkersHeaderItem);
    }

    PNH_HeaderItem GetCustomServerMarkersHeaderItem() {
        return m_CustomServerMarkersHeaderItem;
    }

    PNH_HeaderItem GetServerMarkersHeaderItem() {
        return m_ServerMarkersHeaderItem;
    }

    PNH_HeaderItem GetClientMarkersHeaderItem() {
        return m_ClientMarkersHeaderItem;
    }

    void UpdateContent() {
        array<ref VPPMapItem> items = new array<ref VPPMapItem>;
        foreach (PNH_HeaderItem headerItem : m_HeaderItems) {
            items.Insert(headerItem);
            if (headerItem.m_IsExpanded) {
                foreach (PNH_ListItem listItem : headerItem.m_ListItems) {
                    items.Insert(listItem);
                }
            }
        }
        super.UpdateContent(items);
    }

    override void AddAllWidgets(array<ref VPPMapItem> items) {
        Widget listContentContainer;
        int itemCounter = 0;
        foreach (VPPMapItem item: items) {
            itemCounter++;
            if (!listContentContainer || itemCounter >= 99) {
                itemCounter = 0;
                listContentContainer = GetGame().GetWorkspace().CreateWidgets("PNH_Tablet/GUI/VPP/Layouts/List/ListContentContainer.layout", m_ListContent);
            }
            ItemViewHolder viewHolder = CreateViewHolder(listContentContainer, item);
            FillViewHolder(viewHolder, item);
            m_WidgetItemMap.Insert(viewHolder.m_RootWidget, item);
        }
    }

    override ItemViewHolder CreateViewHolder(Widget root, VPPMapItem item) {
        if (item.IsInherited(PNH_HeaderItem)) {
            return PNH_HeaderItemViewHolder(root);
        } else if (item.IsInherited(PNH_ListItem)) {
            return PNH_ListItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(ItemViewHolder viewHolder, VPPMapItem item) {
        if (item.IsInherited(PNH_HeaderItem) && viewHolder.IsInherited(PNH_HeaderItemViewHolder)) {
            PNH_HeaderItemViewHolder hivh = PNH_HeaderItemViewHolder.Cast(viewHolder);
            PNH_HeaderItem headerItem = PNH_HeaderItem.Cast(item);
            hivh.fillView(headerItem);
        } else if (item.IsInherited(PNH_ListItem) && viewHolder.IsInherited(PNH_ListItemViewHolder)) {
            PNH_ListItemViewHolder livh = PNH_ListItemViewHolder.Cast(viewHolder);
            PNH_ListItem listitem = PNH_ListItem.Cast(item);
            livh.fillView(listitem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentHeight() > y;
    }

    override void OnItemClicked(string widgetName, VPPMapItem item) {

        PNH_HeaderItem headerItem;
        if (widgetName == "collapse_icon" || widgetName == "expand_icon") {
            if (item.IsInherited(PNH_HeaderItem)) {
                headerItem = PNH_HeaderItem.Cast(item);
                headerItem.ToggleExpandCollapse();
                UpdateContent();
            }
        } else if (widgetName == "visible_button") {
            array<PNH_VerticalItem> items = new array<PNH_VerticalItem>;
            if (item.IsInherited(PNH_HeaderItem)) { // Clicou no Cabeçalho
                headerItem = PNH_HeaderItem.Cast(item);
                items.Insert(headerItem);
                foreach (PNH_ListItem listItem : headerItem.m_ListItems) {
                    items.Insert(listItem);
                }
            } else if (item.IsInherited(PNH_ListItem)) { // Clicou no item único
                items.Insert(PNH_VerticalItem.Cast(item));
            }
            
            if (items.Count() > 0 && item.IsInherited(PNH_VerticalItem)) {
                PNH_VerticalItem verticalItem = PNH_VerticalItem.Cast(item);
                switch (verticalItem.m_VisibleState) {
                case VisibleState.NOT_VISIBLE:
                    SetVisible(items, VisibleState.VISIBLE_ONLY_2D);
                    break;
                case VisibleState.VISIBLE_ONLY_2D:
                    SetVisible(items, VisibleState.VISIBLE_2D_AND_3D);
                    break;
                case VisibleState.VISIBLE_2D_AND_3D:
                    SetVisible(items, VisibleState.NOT_VISIBLE);
                    break;
                }
            }
        } else if ((widgetName == "image" || widgetName == "text") && item.IsInherited(PNH_ListItem)) {
            PNH_ListItem list_item = PNH_ListItem.Cast(item);
            if (list_item.m_HeaderParent == GetClientMarkersHeaderItem()) {
                m_Menu.ShowDialog(list_item.m_MarkerIndex, false, false);
            } else if (list_item.m_HeaderParent == GetCustomServerMarkersHeaderItem()) {
                m_Menu.ShowDialog(list_item.m_MarkerIndex, true, false);
            }
        }
    }

    void SetVisible(array<PNH_VerticalItem> items, VisibleState visibleState) {
        array<int> markerIndexes = new array<int>;
        bool isClientMarker = false;
        bool isCustomServerMarker = false;
        foreach (PNH_VerticalItem item : items) {
            item.SetVisible(visibleState);
            if (item.IsInherited(PNH_ListItem)) {
                PNH_ListItem listItem = PNH_ListItem.Cast(item);
                if (listItem.m_HeaderParent == GetClientMarkersHeaderItem()) {
                    isClientMarker = true;
                } else if (listItem.m_HeaderParent == GetCustomServerMarkersHeaderItem()) {
                    isCustomServerMarker = true;
                }
                markerIndexes.Insert(listItem.m_MarkerIndex);
            }
        }
        switch (visibleState) {
        case VisibleState.NOT_VISIBLE:
            m_Menu.EditMarkerVisibility(markerIndexes, false, false, isClientMarker, isCustomServerMarker);
            break;
        case VisibleState.VISIBLE_ONLY_2D:
            m_Menu.EditMarkerVisibility(markerIndexes, true, false, isClientMarker, isCustomServerMarker);
            break;
        case VisibleState.VISIBLE_2D_AND_3D:
            m_Menu.EditMarkerVisibility(markerIndexes, true, true, isClientMarker, isCustomServerMarker);
            break;
        }
    }
}

enum VisibleState {
    NOT_VISIBLE,
    VISIBLE_ONLY_2D,
    VISIBLE_2D_AND_3D
}

class PNH_VerticalItemViewHolder extends ItemViewHolder {
    TextWidget m_TextWidget;
    ImageWidget m_VisibleButton;

    override void init( Widget root) {
        super.init(root);
        m_TextWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget( "text" ) );
        m_VisibleButton = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "visible_button" ) );
    }

    void fillView(PNH_VerticalItem item) {
        switch (item.m_VisibleState) {
        case VisibleState.NOT_VISIBLE:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_not_visible");
            break;
        case VisibleState.VISIBLE_ONLY_2D:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_visible");
            break;
        case VisibleState.VISIBLE_2D_AND_3D:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_visible_3d");
            break;
        }
    }
}

class PNH_VerticalItem extends VPPMapItem {
    string m_Text;
    VisibleState m_VisibleState = VisibleState.VISIBLE_2D_AND_3D;

    void SetVisible(VisibleState visibleState) {
        m_VisibleState = visibleState;
    }
}

class PNH_HeaderItemViewHolder extends PNH_VerticalItemViewHolder {
    ImageWidget m_CollapseIcon;
    ImageWidget m_ExpandIcon;

    void PNH_HeaderItemViewHolder(Widget root) {
        init(GetGame().GetWorkspace().CreateWidgets( "PNH_Tablet/GUI/VPP/Layouts/List/MarkerHeaderItem.layout", root));

        m_CollapseIcon = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "collapse_icon" ) );
        m_ExpandIcon = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "expand_icon" ) );
    }

    void fillView(PNH_HeaderItem headerItem) {
        super.fillView(headerItem);

        m_TextWidget.SetText(headerItem.m_Text);
        m_CollapseIcon.LoadImageFile(0, "set:vpp_map_ui image:icon_expand");
        m_ExpandIcon.LoadImageFile(0, "set:vpp_map_ui image:icon_collapse");
        if (headerItem.m_IsExpanded) {
            m_CollapseIcon.Show(true);
            m_ExpandIcon.Show(false);
        } else {
            m_CollapseIcon.Show(false);
            m_ExpandIcon.Show(true);
        }
    }
}

class PNH_HeaderItem extends PNH_VerticalItem {
    bool m_IsExpanded = true;
    ref array<ref PNH_ListItem> m_ListItems = new array<ref PNH_ListItem>;

    void PNH_HeaderItem(string text) {
        m_Text = text;
    }

    void ToggleExpandCollapse() {
        m_IsExpanded = !m_IsExpanded;
    }

    void AddListItem(ref PNH_ListItem listItem) {
        m_ListItems.Insert(listItem);
    }

    void RemoveListItem(ref PNH_ListItem listItem) {
        m_ListItems.RemoveItem(listItem);
    }

    void Clear() {
        m_ListItems = new array<ref PNH_ListItem>;
    }
}

class PNH_ListItemViewHolder extends PNH_VerticalItemViewHolder {
    ImageWidget m_Image;

    void PNH_ListItemViewHolder(Widget root) {
        init(GetGame().GetWorkspace().CreateWidgets( "PNH_Tablet/GUI/VPP/Layouts/List/MarkerListItem.layout", root));
        m_Image = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "image" ) );
    }

    void fillView(PNH_ListItem listItem) {
        super.fillView(listItem);

        m_TextWidget.SetText(listItem.m_Text);
        m_TextWidget.SetColor(VPPMapMenu.VectorToARGB(listItem.m_Marker.GetColor()));
        m_Image.LoadImageFile(0, listItem.m_MarkerIconPath);
        m_Image.SetColor(VPPMapMenu.VectorToARGB(listItem.m_Marker.GetColor()));
    }
}

class PNH_ListItem extends PNH_VerticalItem {
    ref MarkerInfo m_Marker;
    string m_MarkerIconPath;
    int m_MarkerIndex;
    ref PNH_HeaderItem m_HeaderParent;

    void PNH_ListItem(PNH_HeaderItem headerParent, MarkerInfo marker, int markerIndex) {
        m_HeaderParent = headerParent;
        m_Marker = marker;
        m_MarkerIndex = markerIndex;
        m_Text = marker.GetName();
        m_MarkerIconPath = marker.GetIconPath();
        
        if (marker.IsActive()) {
            if (marker.Is3DActive()) {
                SetVisible(VisibleState.VISIBLE_2D_AND_3D);
            } else {
                SetVisible(VisibleState.VISIBLE_ONLY_2D);
            }
        } else {
            SetVisible(VisibleState.NOT_VISIBLE);
        }
    }
}