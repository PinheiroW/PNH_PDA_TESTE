// Ficheiro: PNH_Tablet/Scripts/5_Mission/PNH_Tablet/App_Map/Components/PNH_IconListAdapter.c

class PNH_IconListAdapter extends PNH_ListAdapter {

    private string m_SelectedIcon;
    private vector m_SelectedColor = Vector(255,255,255);

    void PNH_IconListAdapter(Widget root) {
        super.init("PNH_Tablet/GUI/VPP/Layouts/List/HorizontalList.layout", root);
    }

    void SetSelectedIcon(string icon) {
        m_SelectedIcon = icon;
        UpdateContent();
    }

    void SetSelectedColor(vector color) {
        m_SelectedColor = color;
        UpdateContent();
    }

    string GetSelectedIcon() {
        return m_SelectedIcon;
    }

    void UpdateContent() {
        // Agora usa o novo Agente de Ícones estático
        array<ref PNH_IconInfo> markerIcons = PNH_IconManager.GetIcons();
        array<ref PNH_MapItem> items = new array<ref PNH_MapItem>;
        
        foreach (PNH_IconInfo markerIcon : markerIcons) {
            PNH_IconItem iconItem = new PNH_IconItem();
            iconItem.m_IconPath = markerIcon.Path;
            iconItem.m_SelectedColor = m_SelectedColor;
            
            if (!m_SelectedIcon || m_SelectedIcon == iconItem.m_IconPath) {
                iconItem.m_IsSelected = true;
                m_SelectedIcon = iconItem.m_IconPath;
            }
            items.Insert(iconItem);
        }
        super.UpdateContent(items);
    }

    override void AddAllWidgets(array<ref PNH_MapItem> items) {
        foreach (PNH_MapItem item: items) {
            PNH_ItemViewHolder viewHolder = CreateViewHolder(m_ListContent, item);
            FillViewHolder(viewHolder, item);
            m_WidgetItemMap.Insert(viewHolder.m_RootWidget, item);
        }
    }

    override PNH_ItemViewHolder CreateViewHolder(Widget root, PNH_MapItem item) {
        if (item.IsInherited(PNH_IconItem)) {
            return PNH_IconItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(PNH_ItemViewHolder viewHolder, PNH_MapItem item) {
        if (item.IsInherited(PNH_IconItem) && viewHolder.IsInherited(PNH_IconItemViewHolder)) {
            PNH_IconItemViewHolder vh = PNH_IconItemViewHolder.Cast(viewHolder);
            PNH_IconItem iconItem = PNH_IconItem.Cast(item);
            vh.fillView(iconItem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentWidth() > x;
    }

    override void OnItemClicked(string widgetName, PNH_MapItem item) {
        if (widgetName == "icon_image" || widgetName == "selected_image") {
            if (item.IsInherited(PNH_IconItem)) {
                PNH_IconItem iconItem = PNH_IconItem.Cast(item);
                m_SelectedIcon = iconItem.m_IconPath;
                UpdateContent();
            }
        }
    }
}

class PNH_IconItemViewHolder extends PNH_ItemViewHolder {
    ImageWidget mIconImage;
    ImageWidget mSelectedImage;

    void PNH_IconItemViewHolder(Widget root) {
        super.init(GetGame().GetWorkspace().CreateWidgets("PNH_Tablet/GUI/VPP/Layouts/List/IconListItem.layout", root));

        mIconImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "icon_image" ) );
        mSelectedImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "selected_image" ) );
    }

    void fillView(PNH_IconItem iconItem) {
        mIconImage.LoadImageFile(0, iconItem.m_IconPath);
        mIconImage.SetColor(VPPMapMenu.VectorToARGB(iconItem.m_SelectedColor));
        mSelectedImage.Show(iconItem.m_IsSelected);
        if (iconItem.m_IsSelected) {
            mSelectedImage.LoadImageFile(0, "set:vpp_map_ui image:icon_frame");
        }
    }
}

class PNH_IconItem extends PNH_MapItem {
    bool m_IsSelected = false;
    string m_IconPath;
    vector m_SelectedColor;
}