package taller2.fiuba.cliente.activity;

import android.Manifest;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageView;
import android.widget.Spinner;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;
import taller2.fiuba.cliente.model.DialogoArchivos;
import taller2.fiuba.cliente.model.FileGridAdapter;

/**
 * Actividad principal. Muestra los archivos a los que el usuario tiene acceso.
 * Es la primera actividad que ve el usuario al conectarse al sistema.
 */
public class NavigationActivity extends AppCompatActivity {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);
    /**
     * Codigo resultado de elegir un archivo en el navegador. {@link #uploadFile(String)}
     */
    private static final int PICKFILE_RESULT_CODE = 101;
    /**
     * Codigo resultado de haber realizado una busqueda avanzada
     */
    private static final int ADVANCED_SEARCH_CODE = 102;
    /**
     * Codigo de permisso para escritura de almacenamiento externo.
     */
    private static final int PERMISSION_WRITE_EXTERNAL_STORAGE = 103;
    /**
     * Codigo resultado de {@link ProfileSettingsActivity}
     */
    private static final int PROFILE_SETTINGS_RESULT_CODE = 105;
    /**
     * Codigo de permiso de acceso
     */
    private static final int PERMISSION_ACCESS_FINE_LOCATION = 106;
    private String token, username;
    /**
     * Grilla de archivos del usuario
     */
    GridView gridView;
    /**
     * Lista de archivos del usuario
     */
    static List<String> archivos = new ArrayList();
    /**
     * Selector de criterio de busqueda avanzada
     */
    private Spinner spinner;
    /**
     * Criterios posibles para la busqueda avanzada
     */
    private static final String[] criterios = {"Name", "Owner", "Tag", "Extension"};
    private String ubicacion;
    /**
     * Ubicacion del usuario en otro formato.
     */
    private Location ubicacionLoc;
    /**
     * Variable encargada de ir actualizando la posición actual.
     * Actualiza las variables {@link #ubicacion} y {@link #ubicacionLoc}
     */
    private final LocationListener mLocationListener = new LocationListener() {
        @Override
        public void onStatusChanged(String s, int i, Bundle b){}
        public void onProviderEnabled(String s){}
        public void onProviderDisabled(String s){}
        @Override
        public void onLocationChanged(final Location location) {
            ubicacionLoc = location;
            ubicacion = String.valueOf(location.getLatitude()) + " " + String.valueOf(location.getLongitude());
        }
    };
    /**
     * Variable encargada de proveer datos sobre la posición actual.
     */
    private LocationManager mLocationManager;

    /**
     * Constructor de la actividad principal.
     * Inicializa las variables {@link #token} y {@link #username}.
     * Inicializa {@link #archivos}
     * Llama a {@link #actualizarArchivos()}
     * Inicializa el listener para que los archivos puedan ser clickeados. {@link DialogoArchivos}
     * Inicializa el buscador.
     *
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_navigation);
        token = getIntent().getStringExtra("token");
        username = getIntent().getStringExtra("username");
        //Se pide una lista de los archivos del usuario al server
        actualizarArchivos();

        //Si se clickea un archivo, se abre un dialogo
        gridView.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView parent, View v,
                                    int position, long id) {
                DialogoArchivos diag = new DialogoArchivos();
                Bundle filename = new Bundle();
                //El usuario selecciona una opcion
                filename.putString("filename", archivos.get(position));
                diag.setArguments(filename);
                diag.show(getFragmentManager(), "ss"); //Hay que sacar esto, era para debuggear

            }
        });


        ImageView lupa = (ImageView) findViewById(R.id.lupa);
        lupa.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.out.println("lupa clickeada");
                actualizarArchivos();
                try {
                    String busqueda = ((EditText) findViewById(R.id.searchBar)).getText().toString();
                    String key = ((String) spinner.getSelectedItem()).toUpperCase();
                    System.out.println(busqueda);
                    Request request = new Request("GET", "/files/" + username + "/search/" + key + "/" + busqueda);
                    request.setHeader("conn_token", token);
                    JSONObject response = request.send();
                    JSONArray resultados = response.getJSONArray("search_result");
                    archivos = new ArrayList<String>();
                    for(int i = 0; i < resultados.length() ; i++){
                        archivos.add(resultados.getString(i));
                    }
                    gridView = (GridView) findViewById(R.id.gridView);
                    if (archivos != null) {
                        gridView.setAdapter(new FileGridAdapter(getApplicationContext(), archivos.toArray(new String[archivos.size()])));
                    } else {
                        gridView.setAdapter(new FileGridAdapter(getApplicationContext(), null));
                    }
                } catch (JSONException e){}
            }
        });

        spinner = (Spinner)findViewById(R.id.spinner);
        ArrayAdapter<String>adapter = new ArrayAdapter<String>(NavigationActivity.this,
                android.R.layout.simple_spinner_item, criterios);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        mLocationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000,
                    50, mLocationListener);
        } catch (SecurityException e){}

    }

    /**
     * Al resumir la actividad se actualiza la lista de archivos. {@link #actualizarArchivos()}
     */
    @Override
    public void onResume(){
        actualizarArchivos();
        super.onResume();
    }


    /**
     * Metodo que crea el menu cuando se presiona el boton superior derecho.
     * @param menu El menu a crear
     * @return Si salio bien
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_navigation, menu);
        return true;
    }


    /**
     * Método que responde cuando se clickea un item en el menú.
     * Si se presionó el botón UP, se desconecta del sistema y vuelve a la actividad inicial. {@link #logOut()}
     * Si se presionó Profile Settings, se abre {@link ProfileSettingsActivity}.
     * Si se presionó Search User, se abre {@link UserProfileActivity}
     * Si se presionó Upload File, se abre una ventana de selección de archivo para ser subido.
     * Si se presionó Deleted Files, se abre {@link RecycleBinActivity}.
     * @param item El item presionado
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.profile_settings) {
            Intent profileSettingsIntent = new Intent(getApplicationContext(), ProfileSettingsActivity.class);
            profileSettingsIntent.putExtra("token", token);
            profileSettingsIntent.putExtra("username", username);
            startActivityForResult(profileSettingsIntent, PROFILE_SETTINGS_RESULT_CODE);
            return true;
        }
        if(id == R.id.search_user_profile){
            Intent searchUserProfileIntent = new Intent(getApplicationContext(), UserProfileActivity.class);
            searchUserProfileIntent.putExtra("token", token);
            searchUserProfileIntent.putExtra("username", username);
            startActivity(searchUserProfileIntent);
            return true;

        }
        if (id == android.R.id.home){ //Boton UP (flecha arriba a la izquierda)
            this.logOut();
        }
        if (id == R.id.upload_file){
            Intent fileintent = new Intent(Intent.ACTION_GET_CONTENT);
            fileintent.addCategory(Intent.CATEGORY_OPENABLE);
            fileintent.setType("*/*"); //Este intent es un navegador de archivos
            try {
                startActivityForResult(Intent.createChooser(fileintent, "Select file"), PICKFILE_RESULT_CODE);
            } catch (ActivityNotFoundException e) {}
            return true;
        }
        if (id == R.id.deleted_files){
            Intent recycleBinIntent = new Intent(getApplicationContext(), RecycleBinActivity.class);
            recycleBinIntent.putExtra("token", token);
            recycleBinIntent.putExtra("username", username);
            startActivity(recycleBinIntent);
        }
        return super.onOptionsItemSelected(item);
    }


    /**
     * Al presionar el botón Back, se desloggea del sistema.
     * Llama a {@link #logOut()}
     */
    @Override
    public void onBackPressed() { //Boton BACK (triangulo abajo a la izquierda)
        logOut();
        super.onBackPressed();
    }

    /**
     * Metodo que maneja la finalización de actividades.
     * Si se eligió un archivo para ser subido, se llama a {@link #uploadFile}.
     * Si se eliminó el usuario en {@link ProfileSettingsActivity}, se vuelve a {@link MainActivity}.
     * @param requestCode Código de la actividad iniciada.
     * @param resultCode Código resultado de la actividad.
     * @param data Datos resultados de la actividad.
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case PICKFILE_RESULT_CODE:
                if (resultCode == RESULT_OK) {
                    if(data != null) {
                        Uri FilePath = data.getData();
                        uploadFile(FilePath.getPath());
                    }
                }
                return ;
            case PROFILE_SETTINGS_RESULT_CODE:
                if (resultCode == -1){
                    finish();
                }
        }
    }

    /**
     * Pide al server la lista de archivos a los que el usuario tiene acceso.
     * Tanto propios como compartidos.
     *
     * @return Lista de archivos a los que el usuario puede acceder.
     */
    public JSONArray listFiles(){
        Request request = new Request("GET", "/files/"+username+"/");
        request.setHeader("conn_token", token);
        JSONObject response = request.send();
        JSONArray availableFiles = new JSONArray();
        try {
            JSONArray myFiles = response.getJSONArray("my_file_tokens");
            JSONObject sharedFiles = response.getJSONObject("shared_file_tokens");
            for(int i = 0; i < myFiles.length(); i++){
                availableFiles.put(availableFiles.length(), myFiles.get(i));
            }
            Iterator<String> it = sharedFiles.keys();
            while(it.hasNext()){
                availableFiles.put(availableFiles.length(), sharedFiles.getString(it.next()));
            }

        } catch (JSONException e){}
        return availableFiles;
    }

    /**
     * Se encodea en Base 64 el archivo en el path indicado y se pide al server que lo suba.
     * Se chequean los permisos necesarios. Se piden al usuario en caso de que falten. {@link #verifyStoragePermissions(Activity)}
     * Se actualizan {@link #ubicacionLoc} y {@link #ubicacion}
     * Llama a {@link #actualizarArchivos()}
     * @param path La ruta del archivo a subir
     */
    public void uploadFile(String path){
        JSONObject data = new JSONObject();
        File file = new File(Environment.getExternalStorageDirectory().toString(), path.split(":")[1]);
        try {
            String fname = path.split(":")[1];
            int pos = fname.lastIndexOf("/");
            if (pos > 0) {
                fname = fname.substring(pos+1, fname.length());
            }
            data.put("username", username);
            data.put("filename", fname);
            verifyStoragePermissions(this);
            byte[] arrayB = new byte[(int)file.length()];
            FileInputStream fis = new FileInputStream(file);
            fis.read(arrayB);
            fis.close();
            data.put("content", new String(Base64.encode(arrayB, Base64.DEFAULT)));
            int permission = ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION);
            if (permission != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, PERMISSION_ACCESS_FINE_LOCATION
                );
            }
            ubicacionLoc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (ubicacionLoc != null) {
                ubicacion = String.valueOf(ubicacionLoc.toString());
            }
            data.put("ubicacion", ubicacion);

        } catch (JSONException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Request request = new Request("POST", "/files/"+username+"/", data);
        request.setHeader("conn_token", token);
        request.send();
        actualizarArchivos();
    }

    /**
     * Se desconecta del sistema.
     */
    public void logOut(){
        Request request = new Request("DELETE", "/sessions/"+username);
        request.setHeader("conn_token", token);
        request.send();
    }

    /**
     * Chequea si la aplicación tiene permiso para escribir el almacenamiento externo.
     *
     * Si la aplicación no tiene permiso, se le pide al usuario que lo conceda.
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
        int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    activity,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_WRITE_EXTERNAL_STORAGE
            );
        }
    }

    /**
     * Llama a {@link #listFiles()} y actualizan los archivos mostrados en la cuadricula.
     */
    public void actualizarArchivos(){
        archivos = new ArrayList();
        JSONArray files = listFiles();
        for (int i = 0; i < files.length() ;i++){
            try {
                String next = files.getString(i);
                System.out.println(next); // Debug
                archivos.add(next);
            } catch(JSONException e){}
        }
        gridView = (GridView) findViewById(R.id.gridView);
        if (archivos != null) {
            gridView.setAdapter(new FileGridAdapter(this, archivos.toArray(new String[archivos.size()])));
        } else {
            gridView.setAdapter(new FileGridAdapter(this, null));
        }
    }


}
