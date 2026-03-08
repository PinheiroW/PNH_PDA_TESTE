// Ficheiro: PNH_Tablet/Scripts/5_Mission/PNH_Tablet/App_Map/Components/PNH_ColorListAdapter.c

class PNH_ColorListAdapter extends PNH_ListAdapter {

    private EditDialog m_EditDialog; // Futuramente PNH_MapEdit
    private vector m_SelectedColor;

    void PNH_ColorListAdapter(EditDialog dialog, Widget root) {
        super.init("PNH_Tablet/GUI/VPP/Layouts/List/HorizontalList.layout", root);
        m_EditDialog = dialog;
    }

    void SetSelectedColor(vector color) {
        m_SelectedColor = color;
        UpdateContent();
    }

    vector GetSelectedColor() {
        return m_SelectedColor;
    }

    void UpdateContent() {
        // Consome o novo Agente de Cores estático
        array<vector> colors = PNH_ColorManager.GetColors();
        array<ref PNH_MapItem> items = new array<ref PNH_MapItem>;
        
        foreach (vector color : colors) {
            PNH_ColorItem colorItem = new PNH_ColorItem();
            colorItem.m_Color = color;
            
            if (!m_SelectedColor || m_SelectedColor == color) {
                colorItem.m_IsSelected = true;
                m_SelectedColor = color;
            }
            items.Insert(colorItem);
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
        if (item.IsInherited(PNH_ColorItem)) {
            return PNH_ColorItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(PNH_ItemViewHolder viewHolder, PNH_MapItem item) {
        if (item.IsInherited(PNH_ColorItem) && viewHolder.IsInherited(PNH_ColorItemViewHolder)) {
            PNH_ColorItemViewHolder vh = PNH_ColorItemViewHolder.Cast(viewHolder);
            PNH_ColorItem colorItem = PNH_ColorItem.Cast(item);
            vh.fillView(colorItem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentWidth() > x;
    }

    override void OnItemClicked(string widgetName, PNH_MapItem item) {
        if (widgetName == "color_image" || widgetName == "selected_image") {
            if (item.IsInherited(PNH_ColorItem)) {
                PNH_ColorItem colorItem = PNH_ColorItem.Cast(item);
                m_SelectedColor = colorItem.m_Color;
                m_EditDialog.OnColorSelected(m_SelectedColor);
                UpdateContent();
            }
        }
    }
}

class PNH_ColorItemViewHolder extends PNH_ItemViewHolder {
    ImageWidget mColorImage;
    ImageWidget mSelectedImage;

    void PNH_ColorItemViewHolder(Widget root) {
        super.init(GetGame().GetWorkspace().CreateWidgets("PNH_Tablet/GUI/VPP/Layouts/List/ColorListItem.layout", root));

        mColorImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "color_image" ) );
        mSelectedImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "selected_image" ) );
    }

    void fillView(PNH_ColorItem colorItem) {
        mColorImage.SetColor(VPPMapMenu.VectorToARGB(colorItem.m_Color));
        mSelectedImage.Show(colorItem.m_IsSelected);
        if (colorItem.m_IsSelected) {
            mSelectedImage.LoadImageFile(0, "set:vpp_map_ui image:icon_frame");
        }
    }
}

class PNH_ColorItem extends PNH_MapItem {
    bool m_IsSelected = false;
    vector m_Color = Vector(255, 255, 255);
}