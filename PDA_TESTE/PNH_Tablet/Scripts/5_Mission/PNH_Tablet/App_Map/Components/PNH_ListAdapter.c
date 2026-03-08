// Ficheiro: PNH_Tablet/Scripts/5_Mission/PNH_Tablet/App_Map/Components/PNH_ListAdapter.c

class PNH_ListAdapter extends ScriptedWidgetEventHandler {

    Widget m_ListContent;
    ScrollWidget m_ScrollWidget;
    Widget m_PanelListFrame;

    ref array<ref PNH_MapItem> m_Items = new array<ref PNH_MapItem>; // Matriz de dados base

    ref map<ref Widget, ref PNH_MapItem> m_WidgetItemMap = new map<ref Widget, ref PNH_MapItem>; 

    void init(string layout, Widget root) {
        m_PanelListFrame = root;
        m_ScrollWidget = ScrollWidget.Cast(GetGame().GetWorkspace().CreateWidgets( layout, root ));
        m_ListContent = m_ScrollWidget.FindAnyWidget( "list_content" );

        m_ScrollWidget.SetHandler(this);

        UpdateView();
    }

    void UpdateContent(array<ref PNH_MapItem> items) {
        m_Items = items;
        UpdateView();
    }

    void UpdateView() {
        Widget child = m_ListContent.GetChildren();
        while( child ) {
            Widget lastChild = child;
            child = child.GetSibling();
            m_ListContent.RemoveChild(lastChild);
        }

        m_WidgetItemMap.Clear();
        AddAllWidgets(m_Items);

        m_ListContent.Update();
        m_ScrollWidget.Update();

        if(ShouldShowScrollBar()) {
            m_ScrollWidget.SetAlpha( 1 );
        } else {
            m_ScrollWidget.SetAlpha( 0 );
        }
    }

    void AddAllWidgets(array<ref PNH_MapItem> items);

    PNH_ItemViewHolder CreateViewHolder(Widget root, PNH_MapItem item);

    void FillViewHolder(PNH_ItemViewHolder viewHolder, PNH_MapItem item);

    bool ShouldShowScrollBar();

    void OnItemClicked(string widgetName, PNH_MapItem item);

    override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
        if (button == MouseState.LEFT) {
            PNH_MapItem item = m_WidgetItemMap.Get(w.GetParent());
            if (item) {
                OnItemClicked(w.GetName(), item);
                UpdateView();
                return true;
            }
        }
        return false;
    }
}

class PNH_ItemViewHolder extends Managed {
    ref Widget m_RootWidget;

    void init(Widget root) {
        m_RootWidget = root;
    }

    void fillView(PNH_MapItem item);
}

class PNH_MapItem extends Managed {
}