struct Document {
    int _id;
    double _relevance;
    int _rating;

    Document() = default;
    Document(int id, double relevance, int rating = 0) : _id(id), _relevance(relevance), _rating(rating) {}
};

int main() {
    Document doc{10, 3.5}; // Поле rating будет проинициализировано 0
    // Document docExplicitInitialization(Document{11, 4.5});
}