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
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.swing.JCheckBox;
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
  private final String strGetTaskList = "machinelearning:getTaskList";
  private final String strLoadGen = "machinelearning:loadlastgen";
  
  private List <JCheckBox> taskList = null;
  @InjectPlugin
  public RobotControl parent;
  private de.naoth.rc.dialogs.panels.ExtendedFileChooser fileChooser;

  Map<String, String> commandMap = new HashMap<String, String>();

    /** Creates new form ParameterLearner */
     public ParameterLearner() {
        initComponents();
        // LOL HACKS
        fileChooser = new de.naoth.rc.dialogs.panels.ExtendedFileChooser();
        JCheckBox [] taskArray = {task1, task2, task3, task4, task5, task6, task7, task8, task9, task10, task11, task12, task13, task14, task15};
        taskList = Arrays.asList(taskArray);
        for (int i = 0; i < taskList.size(); i++)
        {
            taskList.get(i).setVisible(false);
        }
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
  
private void getLearningParameterList() {
    if (parent.checkConnected()) {
        if (cbLearningMethod.getSelectedItem() != null) {
            sendCommand(new Command(strMLParameters + ":get"));
        } else {
            jToggleButtonLearn.setSelected(false);
        }
    } else {
        jToggleButtonLearn.setSelected(false);
    }    
}//end getLearningParameterList

private void sendLearningParameters()
{
  if (parent.checkConnected())
  {
     // TODO save under correct name
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
        jToggleButtonKill = new javax.swing.JToggleButton();
        jToggleLoadGen = new javax.swing.JToggleButton();
        jToolBar2 = new javax.swing.JToolBar();
        jToggleButtonLearn = new javax.swing.JToggleButton();
        jToggleButtonList = new javax.swing.JToggleButton();
        cbLearningMethod = new javax.swing.JComboBox();
        jLabel1 = new javax.swing.JLabel();
        task1 = new javax.swing.JCheckBox();
        task2 = new javax.swing.JCheckBox();
        task3 = new javax.swing.JCheckBox();
        task4 = new javax.swing.JCheckBox();
        task5 = new javax.swing.JCheckBox();
        task6 = new javax.swing.JCheckBox();
        task7 = new javax.swing.JCheckBox();
        task8 = new javax.swing.JCheckBox();
        task9 = new javax.swing.JCheckBox();
        task10 = new javax.swing.JCheckBox();
        task11 = new javax.swing.JCheckBox();
        task12 = new javax.swing.JCheckBox();
        task13 = new javax.swing.JCheckBox();
        task14 = new javax.swing.JCheckBox();
        task15 = new javax.swing.JCheckBox();

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

        jToggleButtonKill.setText("Kill current");
        jToggleButtonKill.setFocusable(false);
        jToggleButtonKill.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleButtonKill.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleButtonKill.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonKillActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleButtonKill);

        jToggleLoadGen.setText("LoadLastGen");
        jToggleLoadGen.setFocusable(false);
        jToggleLoadGen.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
        jToggleLoadGen.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
        jToggleLoadGen.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleLoadGenActionPerformed(evt);
            }
        });
        jToolBar1.add(jToggleLoadGen);

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

        task5.setText("Task4");

        task6.setText("Task4");

        task7.setText("Task4");

        task8.setText("Task4");

        task9.setText("Task4");

        task10.setText("Task4");

        task11.setText("Task4");

        task12.setText("Task4");

        task13.setText("Task4");

        task14.setText("Task4");

        task15.setText("Task4");

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
                            .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 250, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(task11)
                            .add(task15)
                            .add(task14)
                            .add(task12)
                            .add(task13))))
                .add(0, 0, Short.MAX_VALUE))
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jLabel1)
                    .add(layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(task1)
                            .add(task2)
                            .add(task4)
                            .add(task3)
                            .add(task5))
                        .add(70, 70, 70)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(task8)
                            .add(task9)
                            .add(task6)
                            .add(task7)
                            .add(task10))))
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
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 161, Short.MAX_VALUE)
                    .add(jScrollPane1))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(filler1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jLabel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 21, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(task1)
                    .add(task6)
                    .add(task11))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(task2)
                    .add(task7)
                    .add(task13))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(task3)
                    .add(task8)
                    .add(task12))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(task4)
                    .add(task9)
                    .add(task14))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(task5)
                    .add(task10)
                    .add(task15))
                .add(30, 30, 30))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jToggleButtonLearnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonLearnActionPerformed
        String method = cbLearningMethod.getSelectedItem().toString();
        if (parent.checkConnected()) {
            if (jToggleButtonLearn.isSelected()) {
                jTextAreaLearningParams.setFocusable(false);
                cbLearningMethod.setFocusable(false);

                sendLearningParameters();

                Map <String, String> tests = getSelectedTests();

                sendCommand("machinelearning:"+method,true,tests);
            } else {
                cbLearningMethod.setFocusable(true);
                jTextAreaLearningParams.setFocusable(true);
                sendCommand("machinelearning:"+method, false);
            }
        } else {
            jToggleButtonLearn.setSelected(false);
        }
    }//GEN-LAST:event_jToggleButtonLearnActionPerformed

    private void cbLearningMethodActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cbLearningMethodActionPerformed
        getLearningParameterList();
    }//GEN-LAST:event_cbLearningMethodActionPerformed

    private void jToggleButtonListActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonListActionPerformed
        if (jToggleButtonList.isSelected()) 
        {
            getLearningParameterList();
            getTaskList();
        }
    }//GEN-LAST:event_jToggleButtonListActionPerformed

    private void jToggleButtonGetInfoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonGetInfoActionPerformed
        Command cmd = new Command("machinelearning:getinfo");
        sendCommand(cmd);
    }//GEN-LAST:event_jToggleButtonGetInfoActionPerformed

    private void jToggleButtonKillActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonKillActionPerformed
        Command cmd = new Command("machinelearning:killcurrent");
        sendCommand(cmd);
    }//GEN-LAST:event_jToggleButtonKillActionPerformed

    private void jToggleLoadGenActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleLoadGenActionPerformed
         if (parent.checkConnected()) {
            Command cmd = new Command (strLoadGen);
            sendCommand(cmd);
     }
    }//GEN-LAST:event_jToggleLoadGenActionPerformed

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
    private javax.swing.JToggleButton jToggleButtonKill;
    private javax.swing.JToggleButton jToggleButtonLearn;
    private javax.swing.JToggleButton jToggleButtonList;
    private javax.swing.JToggleButton jToggleLoadGen;
    private javax.swing.JToolBar jToolBar1;
    private javax.swing.JToolBar jToolBar2;
    private javax.swing.JCheckBox task1;
    private javax.swing.JCheckBox task10;
    private javax.swing.JCheckBox task11;
    private javax.swing.JCheckBox task12;
    private javax.swing.JCheckBox task13;
    private javax.swing.JCheckBox task14;
    private javax.swing.JCheckBox task15;
    private javax.swing.JCheckBox task2;
    private javax.swing.JCheckBox task3;
    private javax.swing.JCheckBox task4;
    private javax.swing.JCheckBox task5;
    private javax.swing.JCheckBox task6;
    private javax.swing.JCheckBox task7;
    private javax.swing.JCheckBox task8;
    private javax.swing.JCheckBox task9;
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
      jToggleButtonList.setSelected(false);
      jToggleButtonGetInfo.setSelected(false);
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
        else if(strCommand.compareTo(strLoadGen) == 0){
            
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
        else if (strCommand.compareTo(strGetInfoML) == 0){
            //String[] infoVals = strResult.split("\n");
            //String[] infoNames = {"best fitness", "fittest gen", "fittest individual", "current gen", "current individual"};
            //String info = "";
            //for (int i = 0; i < infoNames.length; i++)
            //    info += infoNames+":"+infoVals[i];
            // Commented out for now.....
            String info = strResult;
            jTextAreaInfo.setText(info);
            jToggleButtonGetInfo.setSelected(false);
        } else if (strCommand.compareTo("machinelearning:killcurrent") == 0) {
            jToggleButtonKill.setSelected(false);
        } else if (strCommand.compareTo(strGetTaskList) == 0){
            String [] tasks = strResult.split("\n");
            for (int i = 0 ; i < taskList.size(); i++){
                if(i < tasks.length)
                {
                    taskList.get(i).setText(tasks[i]);
                    taskList.get(i).setVisible(true);
                } else {
                    taskList.get(i).setVisible(false);
                }
            }
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
      for (Map.Entry task : tasks.entrySet()) {
          command.addArg((String)task.getKey(), (String)task.getValue());
      }
        
    System.err.println(path + " " + arg);
    sendCommand(command);
  }
  public Map <String, String> getSelectedTests(){
      Map res = new HashMap <String, String>();
      
      for (JCheckBox task : taskList){
            if(task.isSelected()) res.put(task.getText(), "on");
            else res.put(task.getText(), "off");
      }
     
      return res;
  }
  
  public void getTaskList(){
     if (parent.checkConnected()) {
      Command cmd = new Command (strGetTaskList);
      sendCommand(cmd);
     }
      
  }
   
}