#include <mongoc/mongoc.h>

int main(int argc, char* argv[]){
    // local instance connString and port
    const char* uri_string = "mongodb://localhost:27017";
    // pointer to access uri
    mongoc_uri_t* uri;
    // client pointer to access the database
    mongoc_client_t* client;
    // creating the database target pointer
    mongoc_database_t* database;
    // pointer to the collection struct
    mongoc_collection_t* collection;
    // instancing db commands
    bson_t* command, reply, *insert;
    // instancing error types
    bson_error_t error;
    // pointer to receive JSON data
    char* str;
    // init value to confirm ping
    bool retval;
    /*
     * Initializing libmongoc's internals
     */
    mongoc_init();
    /*
     * Enabling option to get Mongo URI from CMD argv
     */
    if(argc > 1){
        uri_string = argv[1];
    };
    /*
     * Creating a URI from the given string
     */
    uri = mongoc_uri_new_with_error(uri_string,&error);
    if(!uri){
      fprintf(stderr,
              "failed to parse URI: %s\n"
              "error message:       %s\n",
              uri_string,
              error.message);
      return EXIT_FAILURE;
    };
    /*
     * Creating the client instance
     */
    client = mongoc_client_new_from_uri(uri);
    if(!client){
      return EXIT_FAILURE;
    };
    /*
     * Registering application name
     */
    mongoc_client_set_appname(client,"ahm-connection");
    /*
     * Conecting in the database and the collection
     */
    database = mongoc_client_get_database(client,"hms");

    collection = mongoc_client_get_collection(client,"hms","empresa");
    /*
     * Ping database , print JSON and insert data
     */
    command = BCON_NEW("ping",BCON_INT32(1));
    // test admin database command
    retval = mongoc_client_command_simple(client,"admin",command,NULL,&reply,&error);
    // throw error
    if(!retval){
        fprintf(stderr,"%s\n",error.message);
        return EXIT_FAILURE;
    };
    // parsing stream to JSON
    str = bson_as_json(&reply,NULL);

    printf("%s\n",str);
    // making schema insertion
    insert = BCON_NEW("Testing",BCON_UTF8("database"));

    if(!mongoc_collection_insert_one(collection,insert,NULL,NULL,&error)){
        fprintf(stderr,"%s\n", error.message);
    };
    // releasing allocated memory
    bson_destroy(insert);
    bson_destroy(&reply);
    bson_destroy(command);
    bson_free(str);

    /*
     * Releasing garbage memo and cleaning up libmongoc
     */
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_uri_destroy(uri);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return EXIT_SUCCESS;
};


