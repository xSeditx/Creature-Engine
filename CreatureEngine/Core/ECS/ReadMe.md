
# Entity Component System
___



___
* Component: ``` Raw data```
___

___
Static Functionality
___
> ``` idTag RegisterComponentType(ComponentCreateFunc _createfunc, ComponentDeleteFunc _deletefunc, size_t _size); ```<br>
> ``` ComponentCreateFunc g_TypeCreateFunction(Component_Handle _id); ```<br>
> ``` ComponentDeleteFunc g_TypeDeleteFunction(Component_Handle _id); ```<br>
> ``` size_t g_TypeSize(Component_Handle _id); ```<br>
> ``` bool is_type_valid(Component_Handle _id); ```<br>


Data Members         | Description
-------------------- | ---------------------
``` ComponentCreateFunc   (* Create_Function);        ```| Function pointer for Allocating the Memory of a Component type<br> Returns an int32 handle which is the components Index in an Array it accepts as the first param where the Component of this types memory is to be stored contiguously.<br> A void pointer is the second argument which points to the Entity it is part of. <br> Also takes a filled in BaseComponent structure defining the Component itself constructing in place a new Entity and returning its Index in first param Memory Array
``` ComponentDeleteFunc   (* Delete_Function);       ```| Function pointer for Freeing the Memory of a Component type. This likely will be altered in the future to delete the Component yet allow its location to be used by future Components of type _Ty
``` size_t SIZE;        ```      | The size of our Component Object
``` int DebugTypeID;				      ```| TypeID for the specific component type
``` std::string ComponentName;           ```| Name of the Component in Human Readable form. 

 

 ___
* Entity: ```List of Components or ID to that specific component```
___

___
* System: ```takes the information and processes the data, likely via a memory manager```
___