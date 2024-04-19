protected void doGet(HttpServletRequest request, HttpServletResponse resp) throws IOException {
    resp.setContentType("text/html");
    resp.setCharacterEncoding("UTF-8");
    
    String firstName = request.getParameter("firstName");
    PrintWriter out = resp.getWriter();
    
    out.append("<html>");
    out.append("<head><title>Search Result</title></head>");
    out.append("<body>");
    out.append("<div>");
    out.append("Search for " + firstName);
    out.append("</div>");
    out.append("</body>");
    out.append("</html>");
}
