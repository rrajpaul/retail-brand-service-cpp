#ifndef MongoUrl_hpp
#define MongoUrl_hpp

#include <string>

static std::string MONGO_URL;

class MongoUrl {
public:
    void setMongUrl(std::string mongo_url) {
        MONGO_URL = mongo_url;
    }

    std::string getMongUrl() {
       return  MONGO_URL;
    }
};

#endif /* MongoUrl_hpp */