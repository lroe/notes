In Java, object serialization is achieved using the `Serializable` interface from the `java.io` package. When a class implements `Serializable`, its objects can be converted into a byte stream, which can then be saved to a file, sent over a network, or stored in a database.

### **Example: Serialization and Deserialization**

#### **1. Define a Serializable Class**

The class must implement the `Serializable` interface.

java

Copy code

`import java.io.*;  // Serializable class class Student implements Serializable {     private static final long serialVersionUID = 1L; // Recommended for Serializable classes     private String name;     private int age;      // Constructor     public Student(String name, int age) {         this.name = name;         this.age = age;     }      // Getters for demonstration purposes     public String getName() {         return name;     }      public int getAge() {         return age;     }      @Override     public String toString() {         return "Student{name='" + name + "', age=" + age + "}";     } }`

---

#### **2. Serialize the Object**

Write the object to a file using `ObjectOutputStream`.

java

Copy code

`public class SerializeExample {     public static void main(String[] args) {         Student student = new Student("Jeevan", 23);          try (FileOutputStream fileOut = new FileOutputStream("student.ser");              ObjectOutputStream out = new ObjectOutputStream(fileOut)) {              out.writeObject(student);             System.out.println("Serialized data is saved in student.ser");          } catch (IOException i) {             i.printStackTrace();         }     } }`

---

#### **3. Deserialize the Object**

Read the object from the file using `ObjectInputStream`.

java

Copy code

`public class DeserializeExample {     public static void main(String[] args) {         Student student;          try (FileInputStream fileIn = new FileInputStream("student.ser");              ObjectInputStream in = new ObjectInputStream(fileIn)) {              student = (Student) in.readObject();             System.out.println("Deserialized Student:");             System.out.println(student);          } catch (IOException | ClassNotFoundException e) {             e.printStackTrace();         }     } }`

---

### **Output**

If you run both programs consecutively:

1. **Serialization Output**:
    
    kotlin
    
    Copy code
    
    `Serialized data is saved in student.ser`
    
2. **Deserialization Output**:
    
    arduino
    
    Copy code
    
    `Deserialized Student: Student{name='Jeevan', age=23}`
    

---

### **Key Notes**

1. **serialVersionUID**:
    
    - It is used for version control during deserialization.
    - If the class structure changes (e.g., adding/removing fields), the deserialization may fail unless the `serialVersionUID` remains consistent.
2. **Non-Serializable Fields**:
    
    - Use the `transient` keyword to exclude fields from serialization.
    
    java
    
    Copy code
    
    `private transient String password;`
    
3. **Custom Serialization**:
    
    - Override the `writeObject` and `readObject` methods to customize the process.