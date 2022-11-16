struct Vector3D {
    Vector3D(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
    Vector3D() = default;

    double x = 0.0;
    double y = 0.0;
    double z = 0.9;

    
};


int main() {
    // В куче создаётся объект типа Vector3D. Адрес этого объекта сохраняется в value_ptr.
    Vector3D* valuePtr = new Vector3D;
    
    // Используем созданный объект по указателю на него
    // *valuePtr = Vector3D(2.2, 15.1, 3.3);
    *valuePtr = {2.2, 15.1, 3.3};

    // Удаляем объект и возвращаем занимаемый им блок памяти обратно в кучу
    delete valuePtr;
}