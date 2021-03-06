package de.naoth.rc.dialogs;

import de.naoth.rc.AbstractDialog;
import de.naoth.rc.RobotControl;
import de.naoth.rc.dataformats.JanusImage;
import de.naoth.rc.dialogs.Tools.Colors;
import de.naoth.rc.manager.ImageManager;
import de.naoth.rc.manager.ObjectListener;
import de.naoth.rc.server.Command;
import de.naoth.rc.server.CommandSender;
import java.awt.*;
import java.awt.event.ItemEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import net.xeoh.plugins.base.annotations.PluginImplementation;
import net.xeoh.plugins.base.annotations.events.Init;
import net.xeoh.plugins.base.annotations.injections.InjectPlugin;

/**
 *
 * @author  CNR
 */
@PluginImplementation
public class ColorCalibrationTool extends AbstractDialog implements ObjectListener<JanusImage>, PropertyChangeListener
{

  @InjectPlugin
  public RobotControl parent;
  @InjectPlugin
  public ImageManager imageManager;

  private ImagePanel imageCanvas;
  private final String cameraAutoParam = "camera:switch_auto_parameters";
  
  private final String showCalibArea = "ImageProcessor:BaseColorClassifier:calibration_areas:";
  private final String showCalibPixel = "ImageProcessor:BaseColorClassifier:set_[OBJECT]_in_image";
  private final String showCalibPixelField = "ImageProcessor:FieldColorClassifier:set_in_image";
  private final String getSetCalibAreaRect = "ParameterList:CalibrationAreaRect_";
  private final String toggleCalibCommand = "ImageProcessor:BaseColorClassifier:calibrate_colors:";
  private final String toggleCalibCommandField = "ImageProcessor:FieldColorClassifier:calibrate:";
  private final String getSetCalibValues = "ParameterList:[COLOR]ColorRegion_[OBJECT]:";
  private final String getSetCalibValuesField = "ParameterList:FieldColorParameters:";
    
  private boolean drawRect = false;
  private Color CalibColor;
  private Integer CalibColorIndex;
  private Colors.ColorClass colorClass;

  private HashMap<Colors.ColorClass, Boolean> classifiedPixelShowStateList;
  
  /** Creates new form ImageViewer */
  public ColorCalibrationTool()
  {
    initComponents();
  }

  @Init
  public void init()
  {

    // setup the image-views
    imageCanvas = new ImagePanel();
    this.imagePanel.add(imageCanvas);
    this.imagePanel.addMouseListener(imageCanvas);
    this.imagePanel.addMouseMotionListener(imageCanvas);
    classifiedPixelShowStateList = new HashMap<Colors.ColorClass, Boolean>();
    for(Colors.ColorClass c: Colors.ColorClass.values())
    {
      classifiedPixelShowStateList.put(c,false);
    }
    
  }//end init

  @Override
  public JPanel getPanel()
  {
    return this;
  }

  /** This method is called from within the constructor to
   * initialize the form.
   * WARNING: Do NOT modify this code. The content of this method is
   * always regenerated by the Form Editor.
   */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        fileChooser = new de.naoth.rc.dialogs.panels.ExtendedFileChooser();
        jToolBar1 = new javax.swing.JToolBar();
        btReceiveImages = new javax.swing.JToggleButton();
        btAutoCameraParameters = new javax.swing.JToggleButton();
        btCalibrate = new javax.swing.JToggleButton();
        jPanel3 = new javax.swing.JPanel();
        originalImageContainer = new javax.swing.JPanel();
        imagePanel = new javax.swing.JPanel();
        coloredObjectChooserPanel = new de.naoth.rc.dialogs.panels.ColoredObjectChooserPanel();
        colorValueSlidersPanel = new de.naoth.rc.dialogs.panels.ColorValueSlidersPanel();

        setPreferredSize(new java.awt.Dimension(800, 663));
        addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                formMouseClicked(evt);
            }
        });

        jToolBar1.setFloatable(false);
        jToolBar1.setRollover(true);

        btReceiveImages.setText("Receive Images");
        btReceiveImages.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btReceiveImagesActionPerformed(evt);
            }
        });
        jToolBar1.add(btReceiveImages);

        btAutoCameraParameters.setText("Auto Camera Params");
        btAutoCameraParameters.setFocusable(false);
        btAutoCameraParameters.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        btAutoCameraParameters.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        btAutoCameraParameters.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btAutoCameraParametersActionPerformed(evt);
            }
        });
        jToolBar1.add(btAutoCameraParameters);

        btCalibrate.setText("Run Calibration");
        btCalibrate.setEnabled(false);
        btCalibrate.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                btCalibrateStateChanged(evt);
            }
        });
        btCalibrate.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                btCalibrateItemStateChanged(evt);
            }
        });
        btCalibrate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btCalibrateActionPerformed(evt);
            }
        });
        jToolBar1.add(btCalibrate);

        jPanel3.setPreferredSize(new java.awt.Dimension(640, 480));

        originalImageContainer.setBackground(java.awt.Color.gray);
        originalImageContainer.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(0, 0, 0)));
        originalImageContainer.setMinimumSize(new java.awt.Dimension(640, 480));
        originalImageContainer.setPreferredSize(new java.awt.Dimension(640, 480));
        originalImageContainer.setLayout(new java.awt.BorderLayout());

        imagePanel.setBackground(java.awt.Color.gray);
        imagePanel.setMinimumSize(new java.awt.Dimension(640, 480));
        imagePanel.setOpaque(false);
        imagePanel.setPreferredSize(new java.awt.Dimension(640, 480));
        imagePanel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                imagePanelMouseClicked(evt);
            }
        });
        imagePanel.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                imagePanelMouseMoved(evt);
            }
        });
        imagePanel.setLayout(new java.awt.BorderLayout());
        originalImageContainer.add(imagePanel, java.awt.BorderLayout.CENTER);

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGap(0, 0, 0)
                .addComponent(originalImageContainer, javax.swing.GroupLayout.PREFERRED_SIZE, 640, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addComponent(originalImageContainer, javax.swing.GroupLayout.PREFERRED_SIZE, 480, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
        );

        coloredObjectChooserPanel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                coloredObjectChooserPanelMouseClicked(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jToolBar1, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(colorValueSlidersPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(coloredObjectChooserPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(142, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jToolBar1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(coloredObjectChooserPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(colorValueSlidersPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())))
        );
    }// </editor-fold>//GEN-END:initComponents
  private void btReceiveImagesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btReceiveImagesActionPerformed

    if(btReceiveImages.isSelected())
    {
      if(parent.checkConnected())
      {
        imageManager.addListener(this);
      }
      else
      {
        btReceiveImages.setSelected(false);
      }
    }
    else
    {
      imageManager.removeListener(this);
    }
  }//GEN-LAST:event_btReceiveImagesActionPerformed

  private void imagePanelMouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_imagePanelMouseMoved
  }//GEN-LAST:event_imagePanelMouseMoved

  private void imagePanelMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_imagePanelMouseClicked
  }//GEN-LAST:event_imagePanelMouseClicked

private void formMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_formMouseClicked
}//GEN-LAST:event_formMouseClicked

private void btAutoCameraParametersActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_btAutoCameraParametersActionPerformed
{//GEN-HEADEREND:event_btAutoCameraParametersActionPerformed

  boolean on = btAutoCameraParameters.isSelected();
  Command cmd = new Command(cameraAutoParam).addArg(on ? "on"  : "off");
  sendCommand(cmd);
}//GEN-LAST:event_btAutoCameraParametersActionPerformed

  private void btCalibrateStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_btCalibrateStateChanged

 }//GEN-LAST:event_btCalibrateStateChanged
  
  private void btCalibrateItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_btCalibrateItemStateChanged
    if(evt.getStateChange() == ItemEvent.SELECTED)
    {
      if(colorClass != null)
      {
        this.colorValueSlidersPanel.setEnabled(false);
//        if(colorValueSlidersPanel.showColoredPixels())
//        {
//          sendShowObjectsPixels(colorClass, "on");
//        }
        sendCommand(new Command(toggleCalibCommand + "reset_data").addArg("on"));
        sendCommand(new Command(toggleCalibCommand + "reset_data").addArg("off"));
        sendColorCalibCommand(colorClass, "on");     
      }
    }
    if(evt.getStateChange() == ItemEvent.DESELECTED)
    {
      btCalibrate.setSelected(false);
      for(Colors.ColorClass c: Colors.ColorClass.values())
      {
        if
        (
          c == Colors.ColorClass.green || 
          c == Colors.ColorClass.white || 
          c == Colors.ColorClass.orange || 
          c == Colors.ColorClass.yellow ||
          c == Colors.ColorClass.skyblue || 
          c == Colors.ColorClass.red || 
          c == Colors.ColorClass.blue
        )
        {
          if(colorValueSlidersPanel.showColoredPixels())
          {
            sendShowObjectsPixels(c, "off");
          }
          sendColorCalibCommand(c, "off");          
        }
      }
      sendGetColorValueCommand(colorClass, "get");
      this.colorValueSlidersPanel.setEnabled(true);
    }
  }//GEN-LAST:event_btCalibrateItemStateChanged

  private void btCalibrateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btCalibrateActionPerformed
  }//GEN-LAST:event_btCalibrateActionPerformed

  private void coloredObjectChooserPanelMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_coloredObjectChooserPanelMouseClicked
  }//GEN-LAST:event_coloredObjectChooserPanelMouseClicked

  @Override
  public void newObjectReceived(JanusImage object)
  {
    imageCanvas.setImage(object);
  }

  @Override
  public void errorOccured(String cause)
  {
    btReceiveImages.setSelected(false);
    imageManager.removeListener(this);
    JOptionPane.showMessageDialog(this,
      cause, "Error", JOptionPane.ERROR_MESSAGE);
  }
  
  private void sendShowObjectsPixels(Colors.ColorClass colorClass, String mode)
  {
    if(colorClass != null)
    {
      String cmdString = showCalibPixel;
      switch(colorClass)
      {
        case green:
            cmdString = showCalibPixelField;
          break;

        case white:
            cmdString = cmdString.replace("[OBJECT]", "lines");
          break;

        case yellow:
        case skyblue:
            cmdString = cmdString.replace("[OBJECT]", "goal");
          break;

        case orange:
            cmdString = cmdString.replace("[OBJECT]", "ball");
          break;

        case red:
         case blue:
            cmdString = cmdString.replace("[OBJECT]", "waistband") + mode;
          break;
      }
      sendCommand(new Command(cmdString).addArg(mode));
    }
  }

  private void sendGetColorValueCommand(Colors.ColorClass colorClass, String mode)
  {
    if(colorClass != null)
    {
      String cmdString = getSetCalibValues;
      switch(colorClass)
      {
        case green:
            cmdString = getSetCalibValuesField + mode;
          break;

        case white:
            cmdString = cmdString.replace("[COLOR]", "white").replace("[OBJECT]", "WhiteLines") + mode;
          break;

        case yellow:
            cmdString = cmdString.replace("[COLOR]", "yellow").replace("[OBJECT]", "YellowGoal") + mode;
          break;

        case skyblue:
            cmdString = cmdString.replace("[COLOR]", "yellow").replace("[OBJECT]", "YellowGoal") + mode;
            //cmdString += cmdString.replace("[COLOR]", "skyblue").replace("[OBJECT]", "BlueGoal") + mode;
          break;

        case orange:
            cmdString = cmdString.replace("[COLOR]", "orange").replace("[OBJECT]", "OrangeBall") + mode;
          break;

        case red:
            cmdString = cmdString.replace("[COLOR]", "pink").replace("[OBJECT]", "PinkWaistBand") + mode;
          break;

        case blue:
            cmdString = cmdString.replace("[COLOR]", "blue").replace("[OBJECT]", "BlueWaistBand") + mode;
          break;
      }
      Command cmd = new Command(cmdString);
      if(mode.equals("set"))
      {
        String[][] values = colorValueSlidersPanel.getControlValues();
        for(int i = 0; i < values.length; i++)
        {
          cmd.addArg(values[i][0], values[i][1]);
        }
      }
      sendCommand(cmd);
    }
  }

  private void sendColorCalibCommand(Colors.ColorClass colorClass, String arg)
  {
    String cmdString = toggleCalibCommand;
    switch(colorClass)
    {
      case green:
            cmdString = toggleCalibCommandField;
        break;

      case white:
          cmdString += "line";
        break;

      case yellow:
          cmdString += "yellow_goal";
        break;

      case skyblue:
          cmdString += "yellow_goal";//"show_blue_goal_area";
        break;

      case orange:
          cmdString += "ball";
        break;

      case red:
          cmdString += "pinkWaistBand";
        break;

      case blue:
          cmdString += "blueWaistBand";
        break;
    }
    sendCommand(new Command(cmdString).addArg(arg));
  }

  @Override
  public void propertyChange(PropertyChangeEvent evt) 
  {
    String property = evt.getPropertyName().replace("ColorCalibrationTool:", "");
    String changedProperty = property.replace(":changed", "");
    String switchedProperty = property.replace(":switched", "");
    
    if(evt.getNewValue() != null && evt.getNewValue() instanceof Boolean )
    {
      String mode = "off";
      if((Boolean) evt.getNewValue())
      {
        mode = "on";
      }
      Colors.ColorClass c = Colors.ColorClass.none;
      
      if(switchedProperty.equals("OrangeBall"))
      {
        c = Colors.ColorClass.orange;
      }
      else if(switchedProperty.equals("YellowGoal"))
      {
        c = Colors.ColorClass.yellow;
      }
      else if(switchedProperty.equals("PinkWaistBand"))
      {
        c = Colors.ColorClass.pink;
      }
      else if(switchedProperty.equals("BlueWaistBand"))
      {
        c = Colors.ColorClass.blue;
      }
      else if(switchedProperty.equals("WhiteLines"))
      {
        c = Colors.ColorClass.white;
      }
      else if(switchedProperty.equals("Field"))
      {
        c = Colors.ColorClass.green;
      }
      sendShowObjectsPixels(c, mode);
      classifiedPixelShowStateList.put(c, (Boolean) evt.getNewValue());     
    }
    else if(evt.getNewValue() != null && evt.getNewValue() instanceof Integer)
    {
      if(changedProperty.equals("OrangeBall"))
      {
        sendGetColorValueCommand(Colors.ColorClass.orange, "set");
      }
      else if(changedProperty.equals("YellowGoal"))
      {
        sendGetColorValueCommand(Colors.ColorClass.yellow, "set");
      }
      else if(changedProperty.equals("PinkWaistBand"))
      {
        sendGetColorValueCommand(Colors.ColorClass.pink, "set");
      }
      else if(changedProperty.equals("BlueWaistBand"))
      {
        sendGetColorValueCommand(Colors.ColorClass.blue, "set");
      }
      else if(changedProperty.equals("WhiteLines"))
      {
        sendGetColorValueCommand(Colors.ColorClass.white, "set");
      }    
      else if(changedProperty.equals("Field"))
      {
        sendGetColorValueCommand(Colors.ColorClass.green, "set");
      }
    }
  }

  private class ImagePanel extends JPanel implements MouseMotionListener, MouseListener//, ComponentListener
  {

    private JanusImage image;
    
    private java.awt.Point[] lowerLeft;
    private java.awt.Point[] upperRight;
    
    private java.awt.Point[] lowerLeftAnim;
    private java.awt.Point[] upperRightAnim;
    
    private Double scale;

    public ImagePanel()
    {
      this.addMouseMotionListener(this);
      this.addMouseListener(this);
//      this.addComponentListener(this);
      this.setSize(imagePanel.getSize());
      this.scale = 2.0;
      
      lowerLeft = new java.awt.Point[Colors.ColorClass.numOfColors.ordinal()];
      upperRight = new java.awt.Point[Colors.ColorClass.numOfColors.ordinal()];
      
      lowerLeftAnim = new java.awt.Point[Colors.ColorClass.numOfColors.ordinal()];
      upperRightAnim = new java.awt.Point[Colors.ColorClass.numOfColors.ordinal()];
      
      for(int i = 0; i < Colors.ColorClass.numOfColors.ordinal(); i++)
      {
        lowerLeft[i] = new java.awt.Point(0, 0);
        upperRight[i] = new java.awt.Point(0, 0);
        lowerLeftAnim[i] = new java.awt.Point(0, 0);
        upperRightAnim[i] = new java.awt.Point(0, 0);
      }
      CalibColorIndex = null;
      CalibColor = Color.black;
    }
    protected BufferedImage backgroundImage;

    public void setImage(JanusImage image)
    {
      this.image = image;
      this.backgroundImage = image.getRgb();
      this.setSize(imagePanel.getSize()); 
      getColor();
      this.repaint();
    }
    
    public JanusImage getImage()
    {
      return this.image;
    }    

    public void getColor()
    {
      Colors.ColorClass newColorClass = coloredObjectChooserPanel.getSelectedColor();
      
      if(newColorClass != colorClass)
      {        
        if(newColorClass == Colors.ColorClass.none)
        {
          btCalibrate.setEnabled(false);
        }
        if(colorClass != null && newColorClass != null )
        {
          colorValueSlidersPanel.removeControls();
        }
        
        for(Colors.ColorClass c: Colors.ColorClass.values())
        {
          if
          (
            c == Colors.ColorClass.green || 
            c == Colors.ColorClass.white || 
            c == Colors.ColorClass.orange || 
            c == Colors.ColorClass.yellow ||
            c == Colors.ColorClass.skyblue || 
            c == Colors.ColorClass.red || 
            c == Colors.ColorClass.blue
          )
          {
            sendShowColorAreaCommand(c, "off");
          }
        }
        btCalibrate.setSelected(false);
        btCalibrate.setEnabled(true);
        sendGetColorValueCommand(newColorClass, "get");
//        colorValueSlidersPanel.removeControls();
//        this.validateTree();
      }
      colorClass = newColorClass;
      if(colorClass != null)
      {
        try 
        {
          CalibColor = Colors.GetColor(colorClass);
        } catch (Exception ex) 
        {
          Logger.getLogger(ColorCalibrationTool.class.getName()).log(Level.SEVERE, null, ex);
        }
        CalibColorIndex = coloredObjectChooserPanel.getSelectedColorIndex();
//        sendGetColorValueCommand(colorClass, "get");
        sendShowColorAreaCommand(colorClass, "on");
      }
      else
      {
        CalibColor = null;
        CalibColorIndex = null;
      }      
    }
    
    private void sendColorAreaCommand(Colors.ColorClass colorClass)
    {
      String cmdString = getSetCalibAreaRect;
      int index = colorClass.ordinal();
      switch(colorClass)
      {
//        case green:
//            cmdString += "LowerField:set";
//          break;
          
        case white:
            cmdString += "WhiteLines:set";
          break;
          
        case yellow:
            cmdString += "YellowGoalPostLeft:set";
          break;
          
        case skyblue:
            cmdString += "YellowGoalPostRight:set";
          break;
          
        case orange:
            cmdString += "OrangeBall:set";
          break;
          
        case red:
            cmdString += "PinkWaistBand:set";
          break;
          
        case blue:
            cmdString += "BlueWaistBand:set";
          break;
        
        default:
          return;
      }
      Command cmd = new Command(cmdString); 
      
      if(lowerLeft[index].x > 639)
      {
        lowerLeft[index].x = 639;
      }
      if(lowerLeft[index].y > 479)
      {
        lowerLeft[index].y = 479;
      }
      
      if(lowerLeft[index].x < 0)
      {
        lowerLeft[index].x = 0;
      }
      if(lowerLeft[index].y < 0)
      {
        lowerLeft[index].y = 0;
      }
      
      if(upperRight[index].x > 639)
      {
        upperRight[index].x = 639;
      }
      if(upperRight[index].y > 479)
      {
        upperRight[index].y = 479;
      }
      
      if(upperRight[index].x < 0)
      {
        upperRight[index].x = 0;
      }
      if(upperRight[index].y < 0)
      {
        upperRight[index].y = 0;
      }
      
      if(lowerLeft[index].x > upperRight[index].x)
      {
        int x = upperRight[index].x;
        upperRight[index].x = lowerLeft[index].x;
        lowerLeft[index].x = x;
      }
      
      if(lowerLeft[index].y > upperRight[index].y)
      {
        int y = upperRight[index].y;
        upperRight[index].y = lowerLeft[index].y;
        lowerLeft[index].y = y;
      }      
      
      cmd.addArg("lowerLeft.x", String.valueOf((int) (lowerLeft[index].x / scale)));
      cmd.addArg("lowerLeft.y", String.valueOf((int) (lowerLeft[index].y / scale)));
      cmd.addArg("upperRight.x", String.valueOf((int) (upperRight[index].x / scale)));
      cmd.addArg("upperRight.y", String.valueOf((int) (upperRight[index].y / scale)));
      sendCommand(cmd);
      
      cmdString = getSetCalibAreaRect;
      index = colorClass.ordinal();
      switch(colorClass)
      {
        case yellow:
            cmdString += "BlueGoalPostLeft:set";
          break;
          
        case skyblue:
            cmdString += "BlueGoalPostRight:set";
          break;
      }      
      cmd = new Command(cmdString);    
      cmd.addArg("lowerLeft.x", String.valueOf(lowerLeft[index].x / 2));
      cmd.addArg("lowerLeft.y", String.valueOf(lowerLeft[index].y / 2));
      cmd.addArg("upperRight.x", String.valueOf(upperRight[index].x / 2));
      cmd.addArg("upperRight.y", String.valueOf(upperRight[index].y / 2));
      sendCommand(cmd);
    }
   
    private void sendShowColorAreaCommand(Colors.ColorClass colorClass, String arg)
    {
      String cmdString = showCalibArea;
      switch(colorClass)
      {
//        case green:
//            cmdString += "show_field_area";
//          break;
          
        case white:
            cmdString += "show_line_area";
          break;
          
        case yellow:
            cmdString += "show_yellow_goal_area";
          break;
          
        case skyblue:
            cmdString += "show_yellow_goal_area";//"show_blue_goal_area";
          break;
          
        case orange:
            cmdString += "show_ball_area";
          break;
          
        case red:
            cmdString += "show_pinkWaistBand_area";
          break;
          
        case blue:
            cmdString += "show_blueWaistBand_area";
          break;
        
        default:
          return;
      }
      
      sendCommand(new Command(cmdString).addArg(arg));
    }

    @Override
    public void mouseMoved(MouseEvent e)
    {
    }//end mouseMoved

    @Override
    public void mouseDragged(MouseEvent e) {
      if(CalibColorIndex != null)
      {
        upperRightAnim[CalibColorIndex] = e.getPoint();
        this.repaint();
      }
     }
    
    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }   

    @Override
    public void mousePressed(MouseEvent e) {
      colorClass = null;
      CalibColor = null;
      CalibColorIndex = null;
      getColor();
      if(CalibColorIndex != null && CalibColor != null)
      {
        lowerLeftAnim[CalibColorIndex] = e.getPoint();
        upperRightAnim[CalibColorIndex] = e.getPoint();
//        lowerLeft[CalibColorIndex] = e.getPoint();
//        upperRight[CalibColorIndex] = e.getPoint();
        drawRect = true;
        this.repaint();
      }
    }

    @Override
    public void mouseReleased(MouseEvent e) {
      if(CalibColorIndex != null && CalibColor != null)
      {
        upperRightAnim[CalibColorIndex] = e.getPoint();
        if(isValidRect(CalibColorIndex))
        {
          lowerLeft[CalibColorIndex] = lowerLeftAnim[CalibColorIndex];
          upperRight[CalibColorIndex] = upperRightAnim[CalibColorIndex];
          sendShowColorAreaCommand(colorClass, "on");
          sendGetColorValueCommand(colorClass, "get");
          sendColorAreaCommand(colorClass);
          this.repaint();
        }
      }
    }   
     
    @Override
    public void mouseClicked(MouseEvent e) {
    }
    
    @Override
    public void paint(Graphics g)
    {
      super.paintComponent(g);
      Graphics2D g2d = (Graphics2D) g;

      if(backgroundImage != null)
      {
        g2d.drawImage(backgroundImage, new AffineTransform(this.scale, 0, 0, this.scale, 0, 0), null);
      }
      if
      (
        drawRect &&
        CalibColorIndex != null && 
        CalibColor != null
      )
      {
        drawRect(g2d, CalibColorIndex, CalibColor);
        try 
        {
          if(CalibColorIndex == Colors.ColorClass.skyblue.ordinal())
          {
            drawRect(g2d, Colors.ColorClass.yellow.ordinal(), Colors.GetColor(Colors.ColorClass.yellow));
          }
          if(CalibColorIndex == Colors.ColorClass.yellow.ordinal())
          {
            drawRect(g2d, Colors.ColorClass.skyblue.ordinal(), Colors.GetColor(Colors.ColorClass.skyblue));
          }
        } 
        catch (Exception ex) 
        {
          Logger.getLogger(ColorCalibrationTool.class.getName()).log(Level.SEVERE, null, ex);
        }
      }
    }//end class paintComponent

    private boolean isValidRect(int index)
    {
      return Math.abs(lowerLeftAnim[index].x - upperRightAnim[index].x) > 1 && Math.abs(lowerLeftAnim[index].y - upperRightAnim[index].y) > 1;
    }
    
    public void drawRect(Graphics2D g2d, int index, Color color)
    {
      if(isValidRect(index))
      {
        int x = lowerLeftAnim[index].x;
        int y = lowerLeftAnim[index].y;

        int width = Math.abs(upperRightAnim[index].x - lowerLeftAnim[index].x);
        int height = Math.abs(upperRightAnim[index].y - lowerLeftAnim[index].y);

        if(upperRightAnim[index].x < lowerLeftAnim[index].x)
        {
          x = upperRightAnim[index].x;
        }

        if(upperRightAnim[index].y < lowerLeftAnim[index].y)
        {
          y = upperRightAnim[index].y;
        }        
        g2d.setColor(color);
        Stroke drawingStroke = new BasicStroke(1);
        Rectangle2D rect = new Rectangle2D.Double(x, y, width, height);
        g2d.setStroke(drawingStroke);
        g2d.draw(rect);
      }
    }
    
//    @Override
//    public void componentResized(ComponentEvent e) {
////      throw new UnsupportedOperationException("Not supported yet.");
//    }
//
//    @Override
//    public void componentMoved(ComponentEvent e) {
////      throw new UnsupportedOperationException("Not supported yet.");
//    }
//
//    @Override
//    public void componentShown(ComponentEvent e) {
////      throw new UnsupportedOperationException("Not supported yet.");
//    }
//
//    @Override
//    public void componentHidden(ComponentEvent e) {
////      throw new UnsupportedOperationException("Not supported yet.");
//    }
  }//end class ImagePanel

  public void addUpdateControls(String name, String result)
  {
    String[] params = result.split("\\n");
    for(int i = 0; i < params.length; i++)
    {
      String[] parts = params[i].split("=");
      if(parts.length == 2)
      {
        colorValueSlidersPanel.addControl(name, parts[0], Integer.parseInt(parts[1]), this, classifiedPixelShowStateList.get(colorClass));
      }
    }
  }
  
  private void sendCommand(Command command)
  {
    final Command commandToExecute = command;
    final ColorCalibrationTool thisFinal = this;
    this.parent.getMessageServer().executeSingleCommand(new CommandSender()
    {
      @Override
      public void handleResponse(byte[] result, Command originalCommand)
      {
        //getSetCalibValues = "ParameterList:[COLOR]ColorRegion_[OBJECT]:";
        String strResult = new String(result);
        if
        (
          originalCommand.getName().contains("ParameterList:") &&
          originalCommand.getName().contains("ColorRegion_") &&
          originalCommand.getName().contains(":get")
        )
        {
          int idx = originalCommand.getName().indexOf("ColorRegion_");
          String name = originalCommand.getName().substring(idx).replace("ColorRegion_", "").replace(":get", "");
          
          addUpdateControls(name, strResult);
        }
        else if((getSetCalibValuesField + "get").equals(originalCommand.getName()))
        {
          addUpdateControls("Field", strResult);
        }
        else if(originalCommand.getName().equals(cameraAutoParam)
          && originalCommand.getArguments().size() >= 1
          && originalCommand.getArguments().keySet().contains("off"))
        {
          String answer = new String(result);
          if(answer.endsWith("parameter were saved"))
          {
            JOptionPane.showMessageDialog(thisFinal,
              "Written camera parameters.", "INFO", JOptionPane.INFORMATION_MESSAGE);
          }
          else
          {
            JOptionPane.showMessageDialog(thisFinal,
                answer, "Camera parameters NOT written", JOptionPane.INFORMATION_MESSAGE);
          }
          
        }

      }//end handleResponse

      @Override
      public void handleError(int code)
      {
        JOptionPane.showMessageDialog(thisFinal,
              "Could not send or receive colortable, code " + code, "ERROR", JOptionPane.ERROR_MESSAGE);
      }

      @Override
      public Command getCurrentCommand()
      {
        return commandToExecute;
      }
    }, command);
  }//end sendCommand

  @Override
  public void dispose()
  {
    //System.out.println("Dispose is not implemented for: " + this.getClass().getName());
    btReceiveImages.setSelected(false);
    imageManager.removeListener(this);
    btCalibrate.setSelected(false);
    colorValueSlidersPanel.removeControls();
  }//end dispose
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JToggleButton btAutoCameraParameters;
    private javax.swing.JToggleButton btCalibrate;
    private javax.swing.JToggleButton btReceiveImages;
    private de.naoth.rc.dialogs.panels.ColorValueSlidersPanel colorValueSlidersPanel;
    private de.naoth.rc.dialogs.panels.ColoredObjectChooserPanel coloredObjectChooserPanel;
    private de.naoth.rc.dialogs.panels.ExtendedFileChooser fileChooser;
    private javax.swing.JPanel imagePanel;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JToolBar jToolBar1;
    private javax.swing.JPanel originalImageContainer;
    // End of variables declaration//GEN-END:variables
}
