import math


radius = 1.0
height = 2.0
num_segments = 16

def generate_cylinder():
   
    def generate_vertices():
        vertices = []
        for i in range(num_segments):
            theta = 2.0 * math.pi * i / num_segments
            x = radius * math.cos(theta)
            z = radius * math.sin(theta)
            vertices.append((x, 0.0, z))
            vertices.append((x, height, z))
        return vertices


    def generate_normals():
        normals = []
        for i in range(num_segments):
            theta = 2.0 * math.pi * i / num_segments
            normal_x = math.cos(theta)
            normal_z = math.sin(theta)
            normals.append((normal_x, 0.0, normal_z))
            normals.append((normal_x, 0.0, normal_z))
        return normals

  
    def generate_faces(vertices):
        faces = []
        for i in range(num_segments):
            base_index = 2 * i + 1
            next_index = (base_index + 2) % (2 * num_segments)
            faces.append((base_index, next_index, next_index + 1, base_index + 1))

       
        bottom_face = list(range(1, num_segments * 2 + 1, 2))
        faces.append(bottom_face)

       
        top_face = list(range(2, num_segments * 2 + 1, 2))
        top_face.append(2)
        faces.append(top_face)

        return faces
    

    vertices = generate_vertices()
    normals = generate_normals()
    faces = generate_faces(vertices)

    return vertices, normals, faces

output_file = "cilindar.obj"


vertices, normals, faces = generate_cylinder()


with open(output_file, "w") as f:
    for vertex in vertices:
        f.write(f"v {vertex[0]:.6f} {vertex[1]:.6f} {vertex[2]:.6f}\n")

    for normal in normals:
        f.write(f"vn {normal[0]:.4f} {normal[1]:.4f} {normal[2]:.4f}\n")

    for face in faces:
        f.write("f")
        for vertex_index in face:
            f.write(f" {vertex_index}")
        f.write("\n")

print(f"Generirana datoteka: {output_file}")
