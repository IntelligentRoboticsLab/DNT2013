/*
 * 
 */

/*
 * DebugRequestPanel.java
 *
 * Created on 29.10.2010, 15:15:34
 */
package de.naoth.rc.dialogs;

import de.naoth.rc.AbstractDialog;
import de.naoth.rc.RobotControl;
import de.naoth.rc.checkboxtree.CheckboxTree;
import de.naoth.rc.checkboxtree.SelectableTreeNode;
import de.naoth.rc.manager.DebugRequestManager;
import de.naoth.rc.manager.ObjectListener;
import de.naoth.rc.server.Command;
import de.naoth.rc.server.CommandSender;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import net.xeoh.plugins.base.annotations.PluginImplementation;
import net.xeoh.plugins.base.annotations.events.Init;
import net.xeoh.plugins.base.annotations.injections.InjectPlugin;

/**
 *
 * @author thomas
 */
@PluginImplementation
public class SimpleDebugRequestPanel extends AbstractDialog
  implements CommandSender, ObjectListener<String[]>
{

  @InjectPlugin
  public RobotControl parent;
  @InjectPlugin
  public DebugRequestManager dbgRequestManager;
  private CheckboxTree debugRequestTree;
  private javax.swing.JScrollPane jScrollPaneCheckBoxTree;

  /** Creates new form DebugRequestPanel */
  public SimpleDebugRequestPanel()
  {
    initComponents();
  }

  /** This method is called from within the constructor to
   * initialize the form.
   * WARNING: Do NOT modify this code. The content of this method is
   * always regenerated by the Form Editor.
   */
  @SuppressWarnings("unchecked")
  // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
  private void initComponents() {

    toolbarMain = new javax.swing.JToolBar();
    btRefresh = new javax.swing.JToggleButton();

    setLayout(new java.awt.BorderLayout());

    toolbarMain.setRollover(true);

    btRefresh.setText("Refresh");
    btRefresh.setFocusable(false);
    btRefresh.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
    btRefresh.setVerticalTextPosition(javax.swing.SwingConstants.BOTTOM);
    btRefresh.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(java.awt.event.ActionEvent evt) {
        btRefreshActionPerformed(evt);
      }
    });
    toolbarMain.add(btRefresh);

    add(toolbarMain, java.awt.BorderLayout.PAGE_START);
  }// </editor-fold>//GEN-END:initComponents

  private void btRefreshActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_btRefreshActionPerformed
  {//GEN-HEADEREND:event_btRefreshActionPerformed
    
    if (btRefresh.isSelected())
    {
      if (parent.checkConnected()) {
        dbgRequestManager.addListener(this);
      } else {
        btRefresh.setSelected(false);
      }
    } else {
      dbgRequestManager.removeListener(this);
    }

  }//GEN-LAST:event_btRefreshActionPerformed
  // Variables declaration - do not modify//GEN-BEGIN:variables
  private javax.swing.JToggleButton btRefresh;
  private javax.swing.JToolBar toolbarMain;
  // End of variables declaration//GEN-END:variables

  @Init
  @Override
  public void init()
  {
    debugRequestTree = new CheckboxTree();
    
    jScrollPaneCheckBoxTree = new JScrollPane(debugRequestTree);
    jScrollPaneCheckBoxTree.setAutoscrolls(true);
    add(jScrollPaneCheckBoxTree, java.awt.BorderLayout.CENTER);
  }

  private void sendCommand(String path, boolean enable)
  {
    Command command = new Command();
    command.setName(path);
    String arg = enable ? "on" : "off";
    command.addArg(arg);

    System.err.println(path + " " + arg);
    send(command);
  }

  private void send(Command command)
  {
    if (parent.checkConnected()) {
      parent.getMessageServer().executeSingleCommand(this, command);
    }
  }

  @Override
  public JPanel getPanel() {
    return this;
  }

  @Override
  public void dispose() {
    remove(debugRequestTree);
  }

  @Override
  public void handleResponse(byte[] result, Command originalCommand)
  {
    System.out.println("handleResponse: " + new String(result));
  }

  @Override
  public void handleError(int code)
  {
    // TODO: handle errors
    System.err.println("handleError: " + code);
  }

  @Override
  public Command getCurrentCommand()
  {
    return new Command("ping");
  }

  @Override
  public void newObjectReceived(String[] object)
  {
    //extendedCheckboxTree.getCheckingModel().setCheckingMode(CheckingMode.SIMPLE);
    for (String str : object)
    {
      String[] tokens = str.split("\\|");

      String tooltip = "NO COMMENT";
      if (tokens.length >= 3) {
        tooltip = tokens[2];
      }
      
      if (tokens.length >= 2)
      {
        boolean selected = tokens[1].equals("1");
        final String path = tokens[0];
        
        debugRequestTree.insertPath(path, ':');
        
        final SelectableTreeNode node = debugRequestTree.getNode(path, ':');
        node.setSelected(selected);
        node.setTooltip(tooltip);

        node.getComponent().addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                sendCommand(path, node.isSelected());
            }
        });
        //extendedCheckboxTree.addPath(tokens[0], tooltip, selected);
      }
    }//end for
    //extendedCheckboxTree.getCheckingModel().setCheckingMode(CheckingMode.PROPAGATE_PRESERVING_UNCHECK);

    btRefresh.setSelected(false);
    dbgRequestManager.removeListener(this);
  }

  @Override
  public void errorOccured(String cause)
  {
    JOptionPane.showMessageDialog(this, cause, "Error", JOptionPane.ERROR_MESSAGE);
  }

  @Override
  public String getDisplayName()
  {
    return "Debug Requests (simple)";
  }
  
}
