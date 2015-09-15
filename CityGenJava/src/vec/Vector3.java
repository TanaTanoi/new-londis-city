package vec;

public class Vector3 {
public final float x,y,z;
	/**
	 * Standard constructor 
	 * @param x
	 * @param y
	 * @param z
	 */
	public Vector3(float x, float y,float z){
		this.x = x;
		this.y = y;
		this.z = z;
	}
	/**
	 * Constructor that takes a 2D vector for the x and y value.
	 * @param v
	 * @param z
	 */
	public Vector3(Vector2 v, float z){
		this.x = v.x;
		this.y = v.y;
		this.z = z;		
	}
	public Vector3 add(Vector3 ov){
		return new Vector3(x+ov.x,y+ov.y,z+ov.z);
	}
	public Vector3 sub(Vector3 ov){
		return new Vector3(x-ov.x,y-ov.y,z-ov.z);
	}
	public Vector3 mul(Vector3 ov){
		return new Vector3(x*ov.x,y*ov.y,z*ov.z);
	}
	/**
	 * Returns this vector divided by the other vector
	 *        this/ov
	 * @param ov
	 * @return
	 */
	public Vector3 div(Vector3 ov){
		return new Vector3(x/ov.x,y/ov.y,z/ov.z);
	}
	public Vector3 add(float f){
		return new Vector3(x+f,y+f,z+f);
	}
	public Vector3 sub(float f){
		return new Vector3(x-f,y-f,z-f);
	}
	public Vector3 mul(float f){
		return new Vector3(x*f,y*f,z*f);
	}
	/**
	 * Returns this vector divided by the input float
	 *      this/f
	 * @param f
	 * @return
	 */
	public Vector3 div(float f){
		return new Vector3(x/f,y/f,z/f);
	}
	
	/**
	 * Calculates the magnitude of the vector
	 * @return
	 */
	public float magnitude(){
		return (float)Math.sqrt(x * x + y * y + z * z);
	}
	
	/**
	 * Returns this vector altered to get a magnitude of one.
	 * @return
	 */
	public Vector3 unitVector(){
		float mag = (float)Math.sqrt(x * x + y * y + z * z);
		return new Vector3(x / mag, y / mag, z / mag);
	}
	
	public float dot(Vector3 ov){
		return x * ov.x + y * ov.y + z * ov.z;
	}
	/**
	 * The Cross Product of this vector and the other vector (ov).
	 * The resulting vector is perpendicular to both this and ov.
	 * @param ov
	 * @return 
	 */
	public Vector3 cross(Vector3 ov) {
		float x = this.y * ov.z - this.z * ov.y;
		float y = this.z * ov.x - this.x * ov.z;
		float z = this.x * ov.y - this.y * ov.x;
		return new Vector3(x, y, z);
	}
	
	@Override
	public boolean equals(Object o){
		if(!(o instanceof Vector3))throw new IllegalArgumentException();
		Vector3 ov = (Vector3)o;
		boolean a = Math.round(x*1000)==Math.round(ov.x*1000);	// \
		boolean b = Math.round(y*1000)==Math.round(ov.y*1000);	//  |--If they are the same within 0.001 accuracy
		boolean c = Math.round(z*1000)==Math.round(ov.z*1000);	// /
		return a&&b&&c;
	}
	
	@Override
	public String toString(){
		StringBuilder sb = new StringBuilder();
		sb.append("Vec: (").append(x).append(",").append(y);
		sb.append(",").append(z).append(")");
		return sb.toString();
	}
	
	@Override 
	public int hashCode(){
		return Integer.parseInt(((int)x*100)+((int)y*100)+((int)z*100)+"");// I guess this works for now?
	}
}
