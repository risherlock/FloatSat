
struct Serializable {
   virtual int serialize(char * const) const  = 0;
   virtual int deserialize(char const * const) = 0;
};



