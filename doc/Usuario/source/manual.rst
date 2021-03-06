============================================================================================================
Manual de Usuario
============================================================================================================
El siguiente manual plantea las distintas acciones que uno usuario debe realizar para poder disfrutar en su completitud los servicios que le ofrece la aplicación Macho-Drive.


Pantalla de Inicio
=============================================================================================================
En la pantalla inicial se ven dos botones, Sign Up y Log In.



.. figure:: screenshots/Main_Activity.png
	:scale: 60%


Sign Up
------------------------------------------------------------------------
Si es su primera vez o no tiene cuenta, ingrese el usuario que desea tener con su password y presione Sign Up. Recuerde solo ingresar letras y numeros.
Se informara por pantalla si el Sign Up fue exitoso o no. 
Esto ultimo puede deberse a haber ingresado campos invalidos o a que el usuario ya este tomado.

Log In
-------------------------------------------------------------------------
Una vez que ya tiene usuario, habiendolo ingresado junto con su password, presione Log In.
Si el usuario no existe o la password es incorrecta, se informara por pantalla.
En caso contrario, se accede a la ventana principal donde se pueden visualizar los archivos.


Pantalla Principal
================================================================================================================================
En la ventana principal se pueden ver todos los archivos a los que tiene acceso. En esta pantalla se pueden seleccionar los archivos, acceder a un menu principal o utilizar la barra de búsqueda.


.. figure:: screenshots/Navigation_Activity.png
	:scale: 60%



Busqueda de archivos
----------------------------------------------------------------------------
En caso de querer realizar una búsqueda se puede presionar la barra de busqueda, lo cual presenta una lista estilo "Drop-down" para seleccionar el criterio de busqueda y una lupa para realizar la busqueda.



.. figure:: screenshots/Search_Criteria.png
	:scale: 60%


Se presentarán por pantalla los resultados.



.. figure:: screenshots/Navigation_Activity_Search_Result.png
	:scale: 60%



Seleccionar archivos
----------------------------------------------------------------------------
Si uno desea tratar un archivo, puede presionar sobre este y aparecerán las opciones:
Download, Edit Details, Delete, Share y Versions.


.. figure:: screenshots/File_Navigation_Dialog.png
	:scale: 60%



Download
----------------------------------------------------------------------------
Si al presionar sobre un archivo se presiona Download, se descarga el archivo seleccionado en la carpeta Downloads del almacenamiento del dispositivo (en caso de no existir, se genera).


Delete
----------------------------------------------------------------------------
Si se presiona Delete, se abre un dialogo que pregunta si esta seguro de querer eliminar el archivo. En caso de presionar cancel o tocar fuera del dialogo, este se cierra y no sucede nada. En caso de tocar OK, el archivo clickeado se manda a la papelera de reciclaje.


.. figure:: screenshots/Delete_File.png
	:scale: 60%




Edit Details
----------------------------------------------------------------------------
Si se presiona Edit Details, se accede a la pantalla de modificación de detalles del archivo. 


.. figure:: screenshots/Edit_Details_Activity.png
	:scale: 60%



En esta pantalla se puede renombrar al archivo al ingresar un nuevo titulo y presionar el boton Rename. 

Tambien se pueden agregar tags al presionar Add Tag, o quitarlos presionando la cruz roja a la derecha del tag que se desea eliminar. La unica finalidad de los tags es poder filtrar archivos segun que tags tienen.


.. figure:: screenshots/Edit_Details_with_Tag.png
	:scale: 60%




Share
----------------------------------------------------------------------------
Si se presiona Share, se accede a la pantalla de modificacion de permisos.

En esta pantalla se puede ver todos los usuarios que tienen tienen acceso al archivo.


.. figure:: screenshots/Share_File_Activity.png
	:scale: 60%



Para compartir el archivo con un usuario, ingrese el nombre del usuario a la izquierda del boton Share y luego presionelo:

En caso de que el usuario no exista, se informa por pantalla.

En caso de que si exista, se agrega a la lista de usuarios con acceso al archivo.
	
Para descompartir el archivo con un usuario, presione la cruz roja a la derecha del nombre del usuario. Se abre un dialogo que pregunta al usuario si esta seguro y solo en caso afirmativo concreta la operacion.


.. figure:: screenshots/Share_File_with_User.png
	:scale: 60%




Versions
---------------------------------------------------------------------------


Si se presiona Versions, se accede a la pantalla de visualizacion de versiones del archivo.


.. figure:: screenshots/Versions_Activity.png
	:scale: 60%



En esta pantalla se observan todas las versiones del archivo. Si se presiona una de estas se abre un dialogo con dos opciones: Download y Delete.

Si se presiona Download, se descarga la version elegida en la carpeta Downloads de la tarjeta SD.

Si se presiona Delete, se elimina la version elegida.


.. figure:: screenshots/Versions_Dialog.png
	:scale: 60%



A su ves existe el botón New Version que permite subir una nueva versión del archivo.


.. figure:: screenshots/Versions_Menu.png
	:scale: 60%



Menu Principal
------------------------------------------------------------------------------------------
Si uno presiona el menu arriba a la derecha apareceran las siguientes opciones:
Profile Settings, Search User Profile, Upload File y Deleted Files,


.. figure:: screenshots/Navigation_Menu.png
	:scale: 60%



Profile Settings
---------------------------------------------------------------------------

Si se presiona Profile Settings, se accede a la pantalla de edicion de perfil de usuario.

En esta se muestran el nombre, email, imagen de perfil, ultima ubicacion conocida y la cuota.


.. figure:: screenshots/Search_User_Profile_Result.png
	:scale: 60%




Para modificar el nombre o el email, ingrese el nuevo valor donde figura el actual y presione Save Changes.

Para modificar la imagen de perfil, presione en la imagen (por default Machoke). Se abre un navegador de archivos, elija su nueva imagen y luego presione save changes.

En caso de presionar Delete User, se le pregunta al usuario si esta seguro y en caso afirmativo, se elimina el usuario y se vuelve a la pantalla inicial (log in, sign up).


.. figure:: screenshots/Delete_Account.png
	:scale: 60%



Search User Profile
--------------------------------------------------------------------------

Si se presiona Search User Profile, se accede a la pantalla de visualizacion de perfiles de usuarios.


.. figure:: screenshots/Search_User_Profile.png
	:scale: 60%



En esta pantalla se debe ingresar el nombre del usuario que desea buscar y presione Search. Se muestran el nombre, email, ultima ubicacion conocida y foto de perfil del mismo.

En caso de no existir el usuario, se informa por pantalla.


.. figure:: screenshots/Profile_Settings.png
	:scale: 60%



Upload File
----------------------------------------------------------------------------
Si se presiona Upload File, se abre un navegador de archivos. Una vez que el usuario selecciono el archivo deseado, este se sube al servidor.


Deleted Files
----------------------------------------------------------------------------
Si se presiona Deleted Files, se accede a la pantalla de papelera de reciclaje.


.. figure:: screenshots/Recycle_Bin.png
	:scale: 60%



En esta pantalla se pueden ver los archivos que se encuentran en la papelera.

Para vaciar la papelera, presione Empty Recycle Bin. Se perderan los archivos en ella.

Para restaurar un archivo, presionelo y elija la opcion Restore (no hay otra).


.. figure:: screenshots/Empty_Recycle_Bin.png
	:scale: 60%


