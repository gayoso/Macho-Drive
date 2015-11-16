package taller2.fiuba.cliente.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import taller2.fiuba.cliente.R;

public class tagsGridAdapter extends BaseAdapter {
    private Context context;
    private final String[] tags;

    public tagsGridAdapter(Context context, String[] tags) {
        this.context = context;
        this.tags = tags;
    }

    public View getView(int position, View convertView, ViewGroup parent) {

        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View gridView;

        if (convertView == null) {

            gridView = inflater.inflate(R.layout.tags, null);

            TextView textView = (TextView) gridView.findViewById(R.id.label);

            textView.setText(tags[position]);

            ImageView icon = (ImageView) gridView .findViewById(R.id.deleteTag);

            icon.setImageResource(R.drawable.redcross);


        } else {
            gridView = (View) convertView;
        }

        return gridView;
    }

    @Override
    public int getCount() {
        return tags.length;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

}