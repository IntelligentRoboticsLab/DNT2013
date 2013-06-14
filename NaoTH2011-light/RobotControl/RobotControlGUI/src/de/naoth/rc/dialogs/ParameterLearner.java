/*
 * ParameterLearner.java
 *
 * Created on Jun 7, 2013, 4:32:39 PM
 */
package de.naoth.rc.dialogs;

import de.naoth.rc.AbstractDialog;
import de.naoth.rc.RobotControl;
import de.naoth.rc.server.Command;
import de.naoth.rc.server.CommandSender;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import net.xeoh.plugins.base.annotations.PluginImplementation;
import net.xeoh.plugins.base.annotations.events.Init;
import net.xeoh.plugins.base.annotations.injections.InjectPlugin;

/**
 *
 * @author auke
 */
@PluginImplementation
public class ParameterLearner extends AbstractDialog implements CommandSender {

  private Command commandToExecute;
  private final String strIKParameters = "ParameterList:IKParameters";
  private final String strMLParameters = "ParameterList:MachineLearningParameters";
  private final String strGetInfoML = "machinelearning:getinfo";
  private boolean learnEnabled = false;
          
  @InjectPlugin
  public RobotControl parent;

  Map<String, String> commandMap = new HashMap<String, String>();

    /** Creates new form ParameterLearner */
     public ParameterLearner() {
        initComponents();
        // LOL HACKS
        cbLearningMethod.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "" }));
        
        jTextAreaLearningParams.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e)
            { 
                int key = e.getKeyCode();
                if (key == KeyEvent.VK_ENTER)
                {
                    sendLearningParameters();

                    int k = jTextAreaLearningParams.getCaretPosition();
                    if(k > 0)
                        jTextAreaLearningParams.setCaretPosition(k-1);
                }
            }
        });
        
        jTextAreaInfo.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e)
            { 
                int key = e.getKeyCode();
                if (key == KeyEvent.VK_ENTER)
                {
                    //saveWalkingParameters();

                    int k = jTextAreaInfo.getCaretPosition();
                    if(k > 0)
                        jTextAreaInfo.setCaretPosition(k-1);
                }
            }
        });
    }

  @Init
  public void init()
  {
    if(parent == null)
    {
      throw (new IllegalArgumentException("\"parent\" was null"));
    }
  }

  @Override
  public JPanel getPanel()
  {
    return this;
  }

private static Command parseTextArea(String cmdName, String text)
{
    return parseTextArea(cmdName, text, "");
} // end parseTextArea

private static Command parseTextArea(String cmdName, String text, String method)
{
    Command cmd = new Command(cmdName);
    
    text = text.replaceAll("( |\t)+", "");
    String[] lines = text.split("(\n)+");
    if (!method.isEmpty())
        method += ".";
    
    for (String l : lines)
    {
      String[] splitted = l.split("=");
      if (splitted.length == 2)
      {
        String key = method + splitted[0].trim();
        String value = splitted[1].trim();
        // remove the last ;
        if (value.charAt(value.length() - 1) == ';')
        {
          value = value.substring(0, value.length() - 1);
        }

        cmd.addArg(key, value);
      }
    } // end for
    return cmd;
} // end parseTextArea
  
  
/*private void saveWalkingParameters() {
  if (parent.checkConnected())
  {
    getWalkingParameterList();
    Command cmd = parseTextArea(strIKParameters + ":set", 
                                this.jTextAreaWalkingParams.getText());
    sendCommand(cmd);
    // stop learning when walking parameters are saved , for now
    // TODO save somewhere externally, or under some specific name, e.g.
    DateFormat dateFormat = new SimpleDateFormat("_yy/MM/dd_HHmmss");
    String savefilename = cbLearningMethod.getSelectedItem().toString() + 
                            dateFormat.format(new Date());
                          
    jToggleButtonLearn.setSelected(false);
  }
  else
  {
    jToggleButtonLearn.setSelected(false);
  }    
}*/

private boolean getParameterList(String strCommand)
{
    if (parent.checkConnected())
    {
      Command cmd = new Command(strCommand);
      sendCommand(cmd);
      return true;
    }
    return false;
}



private void getLearningParameterList() {
    if (cbLearningMethod.getSelectedItem() != null) {
        if(!getParameterList(strMLParameters + ":get"))
        {
            jToggleButtonLearn.setSelected(false);
        }
    }
}//end getLearningParameterList

private void sendLearningParameters()
{
  if (parent.checkConnected())
  {
     // TODO add combobox methsendCommandod name, send correct parameters only
    Command cmd = parseTextArea(strMLParameters + ":set",
            this.jTextAreaLearningParams.getText(), 
            cbLearningMethod.getSelectedItem().toString());

    sendCommand(cmd);
    getLearningParameterList();
  }
  else
  {
    jToggleButtonLearn.setSelected(false);
  }
}

private void sendCommand(Command command)
  {
    commandToExecute = command;
    parent.getMessageServer().executeSingleCommand(this, command);
  }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane3 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextAreaInfo = new javax.swing.JTextArea();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextAreaLearningParams = new javax.swing.JTextArea();
        filler1 = new javax.swing.Box.Filler(new java.awt.Dimension(0, 0), new java.awt.Dimension(0, 0), new java.awt.Dimension(0, 0));
        jToolBar1 = new javax.swing.JToolBar();
        jToggleButtonGetInfo = new javax.swing.JToggleButton();
        jToolBar2 = new javax.swing.JToolBar();
        jToggleButtonLearn = new javax.swing.JToggleButton();
        jToggleButtonList = new javax.swing.JToggleButton();
        cbLearningMethod = new javax.swing.JComboBox();
        jLabel1 = new javax.swing.JLabel();
        task1 = new javax.swing.JCheckBox();
        task2 = new javax.swing.JCheckBox();
        task3 = new javax.swing.JCheckBox();
        task4 = new javax.swing.JCheckBox();

        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null},
                {null, null, null, null}
            },
            new String [] {
                "Title 1", "Title 2", "Title 3", "Title 4"
            }
        ));
        jScrollPane3.setViewportView(jTable1);

        jTextAreaInfo.setColumns(20);
        jTextAreaInfo.setRows(5);
        jScrollPane1.setViewportView(jTextAreaInfo);

        jTextAreaLearningParams.setColumns(20);
        jTextAreaLearningParams.setRows(5);
        jScrollPane2.setViewportView(jTextAreaLearningParams);

        jToolBar1.setRollover(true);

        jToggleButtonGetInfo.setText("GetInfo");
        jToggleButtonGetInfo.setFocusable(false);
        jToggleButtonGetInfo.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonGetInfo.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonGetInfo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonGetInfoActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButtonGetInfo);

        jToolBar2.setRollover(true);

        jToggleButtonLearn.setText("Learn");
        jToggleButtonLearn.setFocusable(false);
        jToggleButtonLearn.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonLearn.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonLearn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonLearnActionPerformed(evt);
            }
        });
        jToolBar2.add(jToggleButtonLearn);

        jToggleButtonList.setText("List");
        jToggleButtonList.setFocusable(false);
        jToggleButtonList.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonList.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonList.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonListActionPerformed(evt);
            }
        });
        jToolBar2.add(jToggleButtonList);

        cbLearningMethod.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Item 1", "Item 2", "Item 3", "Item 4" }));
        cbLearningMethod.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cbLearningMethodActionPerformed(evt);
            }
        });
        jToolBar2.add(cbLearningMethod);

        jLabel1.setText("Evaluation tasks");

        task1.setText("Task1");

        task2.setText("Task2");

        task3.setText("Task3");

        task4.setText("Task4");

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(jToolBar2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(28, 28, 28)
                        .add(filler1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(layout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))))
                .add(0, 0, Short.MAX_VALUE))
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jLabel1)
                    .add(task1)
                    .add(task2)
                    .add(task3)
                    .add(task4))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 25, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jToolBar2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 25, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 188, Short.MAX_VALUE)
                    .add(jScrollPane1))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(filler1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jLabel1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(task1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(task2)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(task3)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(task4)
                .add(30, 30, 30))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jToggleButtonLearnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonLearnActionPerformed
    String method = cbLearningMethod.getSelectedItem().toString();
        if(learnEnabled){
                learnEnabled = false;
                cbLearningMethod.setFocusable(true);
                 jTextAreaLearningParams.setFocusable(true);
                sendCommand("machinelearning:"+method, false);
                return;
            }
    if (jToggleButtonLearn.isSelected())
        {
        if (parent.checkConnected()) {
            learnEnabled = true;
            
            jTextAreaLearningParams.setFocusable(false);
            cbLearningMethod.setFocusable(false);
            
            sendLearningParameters();
            
            Map <String, String> tests = getSelectedTests();
            
            sendCommand("machinelearning:"+method,true,tests);
            
            
         // TODO create manager which refreshes commands, see debugrequestmanager
          } else {
            jToggleButtonLearn.setSelected(false);
          }
        } else {
          // TODO remove listener
        }
    }//GEN-LAST:event_jToggleButtonLearnActionPerformed

    private void cbLearningMethodActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cbLearningMethodActionPerformed
        getLearningParameterList();
    }//GEN-LAST:event_cbLearningMethodActionPerformed

    private void jToggleButtonListActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonListActionPerformed
        if (jToggleButtonLearn.isSelected()) 
        {
            getLearningParameterList();
        }
    }//GEN-LAST:event_jToggleButtonListActionPerformed

    private void jToggleButtonGetInfoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonGetInfoActionPerformed
        Command cmd = new Command();
        cmd.setName("machinelearning:getinfo");
        sendCommand(cmd);
        
    }//GEN-LAST:event_jToggleButtonGetInfoActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(ParameterLearner.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {

            @Override
            public void run() {
                new ParameterLearner().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox cbLearningMethod;
    private javax.swing.Box.Filler filler1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JTable jTable1;
    private javax.swing.JTextArea jTextAreaInfo;
    private javax.swing.JTextArea jTextAreaLearningParams;
    private javax.swing.JToggleButton jToggleButtonGetInfo;
    private javax.swing.JToggleButton jToggleButtonLearn;
    private javax.swing.JToggleButton jToggleButtonList;
    private javax.swing.JToolBar jToolBar1;
    private javax.swing.JToolBar jToolBar2;
    private javax.swing.JCheckBox task1;
    private javax.swing.JCheckBox task2;
    private javax.swing.JCheckBox task3;
    private javax.swing.JCheckBox task4;
    // End of variables declaration//GEN-END:variables

  @Override
  public void handleResponse(byte[] result, Command originalCommand)
  {
    String strResult = new String(result);
    if (strResult.contains("[DebugServer] unknown command: ") || strResult.contains("Unknown command"))
    {
      JOptionPane.showMessageDialog(this, "Can not send parameters!", 
              "Parameter Learner", JOptionPane.ERROR_MESSAGE);
      jToggleButtonLearn.setSelected(false);
    }
    else
    {
        String strCommand = originalCommand.getName();
        if(strCommand.compareTo(strIKParameters + ":get") == 0)
        {
            int k = jTextAreaInfo.getCaretPosition();
            jTextAreaInfo.setText(strResult);
            try {
                jTextAreaInfo.setCaretPosition(k);
            } catch(IllegalArgumentException ex) { /* do nothing */ }
        } 
        else if(strCommand.compareTo(strMLParameters + ":get") == 0)
        {
            int k = jTextAreaLearningParams.getCaretPosition();
            
            String selectedMethod = null;
            if (cbLearningMethod.getSelectedItem() != null)
                selectedMethod = cbLearningMethod.getSelectedItem().toString();
            
            if (jToggleButtonList.isSelected()) {
                cbLearningMethod.removeAllItems();
                String[] mlParameterList = strResult.split("\n");

                ArrayList <String> methods = new ArrayList <String>();

                for (String method : mlParameterList)
                {
                    String methodname = method.split("\\.")[0];
                    if (!methods.contains(methodname)) {
                        methods.add(methodname);
                    }
                }
                for (String method : methods)
                {
                    cbLearningMethod.addItem(method);
                }
             }
             if(selectedMethod != null) {
                 String parameterList = getMethodParameters(selectedMethod, strResult);
                 jTextAreaLearningParams.setText(parameterList);
                 
                 // Reset method lister combo box if items were removed
                 if(jToggleButtonList.isSelected()) {
                    cbLearningMethod.setSelectedItem(selectedMethod);
                 }
             }
             jToggleButtonList.setSelected(false);

             try {
                jTextAreaLearningParams.setCaretPosition(k);
             } catch(IllegalArgumentException ex) { /* do nothing */ }

        }
        else if (strCommand.compareTo(strGetInfoML + ":get") == 0){
            String[] infoVals = strResult.split("\n");
            String[] infoNames = {"best fitness", "fittest gen", "fittest individual", "current gen", "current individual"};
            String info = "";
            for (int i = 0; i < infoNames.length; i++)
                info += infoNames+":"+infoVals[i];
            jTextAreaInfo.setText(info);
            
        }
    }
  }//end handleResponse

  @Override
  public void handleError(int code)
  {
    jToggleButtonLearn.setSelected(false);
    JOptionPane.showMessageDialog(this,
              "Error occured, code " + code, "ERROR", JOptionPane.ERROR_MESSAGE);
  }//end handleError

  @Override
  public Command getCurrentCommand()
  {
    return commandToExecute;
  }
    
  public String getMethodParameters(String method, String parameters){
      String [] params = parameters.split("\n");
      String [] split = new String[2];  
      String res = "";
      
      for(String p : params){
          split = p.split("\\.", 2);
          if (split[0].equals(method)) res+=split[1]+"\n"; 
      }
      
      return res;
  }
  
   private void sendCommand(String path, boolean enable)
  {
    Command command = new Command();
    command.setName(path);
    String arg = enable ? "on" : "off";
    command.addArg(arg);

    System.err.println(path + " " + arg);
    sendCommand(command);
  }
   
   
    private void sendCommand(String path, boolean enable, Map <String, String> tasks)
  {
    Command command = new Command();
    command.setName(path);
    String arg = enable ? "on" : "off";
    command.addArg(arg);
    
    Iterator i = tasks.entrySet().iterator();
    
    while(i.hasNext()){
        Map.Entry task = (Map.Entry) i.next();
        command.addArg((String)task.getKey(), (String)task.getValue());
    }
        

    System.err.println(path + " " + arg);
    sendCommand(command);
  }
  public Map <String, String> getSelectedTests(){
      Map res = new HashMap <String, String>();
      if(task1.isSelected()) res.put(task1.getText(), "on");
      else res.put(task1.getText(), "off");
      if(task2.isSelected()) res.put(task2.getText(), "on");
      else res.put(task2.getText(), "off");
      if(task3.isSelected()) res.put(task3.getText(), "on");
      else res.put(task3.getText(), "off");
      if(task4.isSelected()) res.put(task4.getText(), "on");
      else res.put(task4.getText(), "off");
      return res;
  }
   
}