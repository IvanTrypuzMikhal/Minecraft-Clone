float vertices[] = {
    //   X      Y      Z       U     V
    // --- Cara Frontal (Lateral - Inf. Izq.) ---
    -0.2f, -0.2f,  0.2f,    0.0f, 0.0f,  // Inferior Izquierda
     0.2f, -0.2f,  0.2f,    0.5f, 0.0f,  // Inferior Derecha
     0.2f,  0.2f,  0.2f,    0.5f, 0.5f,  // Superior Derecha
     0.2f,  0.2f,  0.2f,    0.5f, 0.5f,  // Superior Derecha
    -0.2f,  0.2f,  0.2f,    0.0f, 0.5f,  // Superior Izquierda
    -0.2f, -0.2f,  0.2f,    0.0f, 0.0f,  // Inferior Izquierda

    // --- Cara Trasera (Lateral - Inf. Izq.) ---
    -0.2f, -0.2f, -0.2f,    0.5f, 0.0f,  // Inferior Izquierda
    -0.2f,  0.2f, -0.2f,    0.5f, 0.5f,  // Superior Izquierda
     0.2f,  0.2f, -0.2f,    0.0f, 0.5f,  // Superior Derecha
     0.2f,  0.2f, -0.2f,    0.0f, 0.5f,  // Superior Derecha
     0.2f, -0.2f, -0.2f,    0.0f, 0.0f,  // Inferior Derecha
    -0.2f, -0.2f, -0.2f,    0.5f, 0.0f,  // Inferior Izquierda

    // --- Cara Izquierda (Lateral - Inf. Izq.) ---
    -0.2f,  0.2f,  0.2f,    0.5f, 0.5f,  // Superior Frontal
    -0.2f,  0.2f, -0.2f,    0.0f, 0.5f,  // Superior Trasera
    -0.2f, -0.2f, -0.2f,    0.0f, 0.0f,  // Inferior Trasera
    -0.2f, -0.2f, -0.2f,    0.0f, 0.0f,  // Inferior Trasera
    -0.2f, -0.2f,  0.2f,    0.5f, 0.0f,  // Inferior Frontal
    -0.2f,  0.2f,  0.2f,    0.5f, 0.5f,  // Superior Frontal

    // --- Cara Derecha (Lateral - Inf. Izq.) ---
     0.2f,  0.2f,  0.2f,    0.0f, 0.5f,  // Superior Frontal
     0.2f, -0.2f,  0.2f,    0.0f, 0.0f,  // Inferior Frontal
     0.2f, -0.2f, -0.2f,    0.5f, 0.0f,  // Inferior Trasera
     0.2f, -0.2f, -0.2f,    0.5f, 0.0f,  // Inferior Trasera
     0.2f,  0.2f, -0.2f,    0.5f, 0.5f,  // Superior Trasera
     0.2f,  0.2f,  0.2f,    0.0f, 0.5f,  // Superior Frontal

     // --- Cara Inferior (Tierra - Sup. Izq.) ---
     -0.2f, -0.2f, -0.2f,    0.0f, 0.5f,  // Trasera Izquierda
      0.2f, -0.2f, -0.2f,    0.5f, 0.5f,  // Trasera Derecha
      0.2f, -0.2f,  0.2f,    0.5f, 1.0f,  // Frontal Derecha
      0.2f, -0.2f,  0.2f,    0.5f, 1.0f,  // Frontal Derecha
     -0.2f, -0.2f,  0.2f,    0.0f, 1.0f,  // Frontal Izquierda
     -0.2f, -0.2f, -0.2f,    0.0f, 0.5f,  // Trasera Izquierda

     // --- Cara Superior (Césped - Inf. Der.) ---
     -0.2f,  0.2f, -0.2f,    0.5f, 0.5f,  // Trasera Izquierda
     -0.2f,  0.2f,  0.2f,    0.5f, 0.0f,  // Frontal Izquierda
      0.2f,  0.2f,  0.2f,    1.0f, 0.0f,  // Frontal Derecha
      0.2f,  0.2f,  0.2f,    1.0f, 0.0f,  // Frontal Derecha
      0.2f,  0.2f, -0.2f,    1.0f, 0.5f,  // Trasera Derecha
     -0.2f,  0.2f, -0.2f,    0.5f, 0.5f,  // Trasera Izquierda
};