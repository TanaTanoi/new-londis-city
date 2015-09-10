package building;
import static org.lwjgl.glfw.Callbacks.errorCallbackPrint;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryUtil.NULL;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import vec.*;
public class Generator {

	/**
	 * 
	 * @param floor - x,z
	 * @param n
	 */
	public static void renderRandomBlock(List<Vector2> floor, int n){
		List<Vector3> bot = new ArrayList<Vector3>();
		List<Vector3> top = new ArrayList<Vector3>();
		float height = (float)Math.random()*2;
		for(Vector2 v2: floor){
			bot.add(new Vector3(v2.x,0,v2.y));
			top.add(new Vector3(v2.x,height,v2.y));
		}
		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			
			//Draw each side of the building
			for(int i = 0; i < n;i++){
				Vector3 topR = top.get(i);			//For the normals: 	1
				Vector3 topL = top.get((i+1)%(n));	//					2
				Vector3 botL = bot.get((i+1)%(n));	//					3
				Vector3 botR = bot.get(i);
				
				glVertex3f(topR.x, topR.y, topR.z);
				glVertex3f(topL.x, topL.y, topL.z);
				glVertex3f(botL.x, botL.y, botL.z);
				glVertex3f(botR.x, botR.y, botR.z);
				Vector3 normal = botL.sub(topL).cross(topL.sub(topR));
				glNormal3f(normal.x, normal.y, normal.z);
				
				
				
			}
			//Roof 
			for(Vector3 roofPart:top){
				glVertex3f(roofPart.x,roofPart.y,roofPart.z);
			}
			glNormal3f(0, 1, 0);//up vector for roof
			
			
		glEnd();
	}
	
}
