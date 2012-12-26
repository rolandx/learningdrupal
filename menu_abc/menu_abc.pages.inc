<?php 
/*
 *菜单项menu_abc的回调函数
 */
function menu_abc_callback_page(){
    $render_array=array();
    $render_array['#markup'] = t('菜单ABC页面内容');
    return $render_array;
}

/*
 *菜单项menu_abc/sub的回调函数
 */
function menu_abc_sub_callback_page(){
    $render_array=array();
    //路径中的其他部分作为参数传递过来
    $render_array['#markup'] = t('菜单ABC子页面内容@arg1 @arg2',array('@arg1'=>$arg1,'@arg2'=>$arg2,'@arg3'=>$arg3,'@arg4'=>$arg4));
    return $render_array;
}


/*
 *实现hook_permission().
 */
function menu_abc_permission(){
       $perms = array(
            'access abc' => array(
             'title' => t('访问菜单ABC示例页面'),
            ),
          );
       return $perms;

}

/*
 *页面标题的回调
 */
 function menu_abc_my_title(){
    global $user;
    $title = $user->name.t('的主页');
    return $title;
}
